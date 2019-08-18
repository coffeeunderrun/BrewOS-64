#include <Uefi.h>
#include <Guid/FileInfo.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/PeCoffLib.h>
#include <Library/UefiLib.h>
#include <Protocol/LoadedImage.h>
#include <Protocol/SimpleFileSystem.h>
#include <Uefi/UefiSpec.h>

static EFI_STATUS load_kernel_image(IN CHAR16 *file_name);

static EFI_BOOT_SERVICES *boot_services = NULL;
static EFI_LOADED_IMAGE_PROTOCOL *loaded_image = NULL;

typedef void(__cdecl *func_ptr)(void);

func_ptr kernel_main;

/*
 * brew_boot_main
 */
EFI_STATUS EFIAPI brew_boot_main(IN EFI_HANDLE image_handle, IN EFI_SYSTEM_TABLE *system_table)
{
	ASSERT(image_handle != NULL);
	ASSERT(system_table != NULL);

	boot_services = system_table->BootServices;

	// Turn off watchdog timer
	EFI_STATUS status = boot_services->SetWatchdogTimer(0, 0, 0, NULL);
	if (EFI_ERROR(status))
	{
		DEBUG((EFI_D_ERROR, "\nFailed to disable watchdog timer. (%r)\n", status));
	}

	// Get image handle for BrewBoot
	status = boot_services->HandleProtocol(image_handle, &gEfiLoadedImageProtocolGuid, (void **)&loaded_image);
	if (EFI_ERROR(status))
	{
		DEBUG((EFI_D_ERROR, "\nFailed to boot image handle. (%r)\n", status));
		return status;
	}

	// Load kernel
	status = load_kernel_image(L"Kernel.exe");
	if (EFI_ERROR(status))
	{
		return status;
	}

	// Don't need boot services any more
	status = boot_services->ExitBootServices(image_handle, 0);
	if (EFI_ERROR(status))
	{
		DEBUG((EFI_D_ERROR, "\nFailed to exit boot services. (%r)\n", status));
	}
	
	// Jump to kernel
	kernel_main();

	return EFI_SUCCESS;
}

/*
 * load_kernel_image
 */
static EFI_STATUS load_kernel_image(IN CHAR16 *file_name)
{
	ASSERT(boot_services != NULL);
	ASSERT(file_name != NULL);

	// Get file system
	EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *file_system = NULL;
	EFI_STATUS status = boot_services->HandleProtocol(loaded_image->DeviceHandle, &gEfiSimpleFileSystemProtocolGuid, (void **)&file_system);
	if(EFI_ERROR(status))
	{
		DEBUG((EFI_D_ERROR, "\nFailed to get file system. (%r)\n", status));
		return status;
	}

	// Get file handle for volume root
	EFI_FILE_PROTOCOL *volume = NULL;
	status = file_system->OpenVolume(file_system, &volume);
	if(EFI_ERROR(status))
	{
		DEBUG((EFI_D_ERROR, "\nFailed to open boot volume. (%r)\n", status));
		return status;
	}
	
	// Get file handle for kernel
	EFI_FILE_PROTOCOL *file = NULL;
	status = volume->Open(volume, &file, file_name, EFI_FILE_MODE_READ, EFI_FILE_READ_ONLY);
	if (EFI_ERROR(status))
	{
		DEBUG((EFI_D_ERROR, "\nFailed to load kernel '%s'. (%r)\n", file_name, status));
		if (status == EFI_NOT_FOUND)
		{
			Print(L"%s not found.\r\n", file_name);
		}
		return status;
	}

	// Allocate buffer for file info
	UINTN file_info_size = 0x100;
	EFI_FILE_INFO *file_info = NULL;
	status = boot_services->AllocatePool(EfiBootServicesData, file_info_size, (void **)&file_info);
	if (EFI_ERROR(status))
	{
		DEBUG((EFI_D_ERROR, "\nFailed to allocate file info buffer. (%r)\n", status));
		return status;
	}

	// Get file info
	status = file->GetInfo(file, &gEfiFileInfoGuid, &file_info_size, file_info);
	if (EFI_ERROR(status))
	{
		DEBUG((EFI_D_ERROR, "\nFailed to get file info for kernel. (%r)\n", status));
		return status;
	}

	UINTN file_size = file_info->FileSize;

	// Free file info buffer
	status = boot_services->FreePool(file_info);
	if (EFI_ERROR(status))
	{
		DEBUG((EFI_D_ERROR, "\nFailed to dispose of file info buffer. (%r)\n", status));
		return status;
	}

	// Allocate file read buffer
	void *file_buffer = NULL;
	status = boot_services->AllocatePool(EfiBootServicesData, file_size, (void **)&file_buffer);
	if (EFI_ERROR(status))
	{
		DEBUG((EFI_D_ERROR, "\nFailed to allocate file read buffer. (%r)\n", status));
		return status;
	}

	// Read file into memory
	status = file->Read(file, &file_size, file_buffer);
	if (EFI_ERROR(status))
	{
		DEBUG((EFI_D_ERROR, "\nFailed to read kernel into file read buffer. (%r)\n", status));
		return status;
	}

	PE_COFF_LOADER_IMAGE_CONTEXT image_context;
	ZeroMem(&image_context, sizeof image_context);
	image_context.Handle = file_buffer;
	image_context.ImageRead = PeCoffLoaderImageReadFromMemory;

	// Get image info
	status = PeCoffLoaderGetImageInfo(&image_context);
	if (EFI_ERROR(status))
	{
		DEBUG((EFI_D_ERROR, "\nFailed to get image info. (%r)\n", status));
		return status;
	}

	// Allocate image buffer
	void *image_buffer = NULL;
	status = boot_services->AllocatePool(EfiBootServicesCode, image_context.ImageSize + image_context.SectionAlignment * 2, (void **)&image_buffer);
	if (EFI_ERROR(status))
	{
		DEBUG((EFI_D_ERROR, "\nFailed to allocate image buffer. (%r)\n", status));
		return status;
	}

	// Make sure image base address is section aligned
	image_context.ImageAddress = (EFI_PHYSICAL_ADDRESS)image_buffer;
	image_context.ImageAddress += image_context.SectionAlignment - 1;
	image_context.ImageAddress &= ~((EFI_PHYSICAL_ADDRESS)image_context.SectionAlignment - 1);

	// Load image from file read buffer
	status = PeCoffLoaderLoadImage(&image_context);
	if (EFI_ERROR(status))
	{
		DEBUG((EFI_D_ERROR, "\nFailed to load kernel into image buffer. (%r)\n", status));
		return status;
	}

	// Apply relocation to image
	image_context.DestinationAddress = image_context.ImageAddress;
	status = PeCoffLoaderRelocateImage(&image_context);
	if (EFI_ERROR(status))
	{
		DEBUG((EFI_D_ERROR, "\nFailed to apply relocation to image. (%r)\n", status));
		return status;
	}

	// Save image entry point
	kernel_main = (func_ptr)image_context.EntryPoint;

	// Dispose of file read buffer
	status = boot_services->FreePool(file_buffer);
	if (EFI_ERROR(status))
	{
		DEBUG((EFI_D_ERROR, "\nFailed to dispose of file read buffer. (%r)\n", status));
		return status;
	}

	// Close file handle
	status = file->Close(file);
	if (EFI_ERROR(status))
	{
		DEBUG((EFI_D_ERROR, "\nFailed to close file handle. (%r)\n", status));
		return status;
	}

	// Close volume handle
	status = volume->Close(volume);
	if (EFI_ERROR(status))
	{
		DEBUG((EFI_D_ERROR, "\nFailed to close boot volume. (%r)\n", status));
		return status;
	}

	return EFI_SUCCESS;
}