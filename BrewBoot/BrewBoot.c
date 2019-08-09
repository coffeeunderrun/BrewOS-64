#include <Uefi.h>
#include <Guid/FileInfo.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/PeCoffExtraActionLib.h>
#include <Library/PeCoffLib.h>
#include <Library/UefiLib.h>
#include <Protocol/LoadedImage.h>
#include <Protocol/SimpleFileSystem.h>
#include <Uefi/UefiSpec.h>

static EFI_STATUS load_kernel_image(IN CHAR16 *file_name);

static EFI_BOOT_SERVICES *boot_services = NULL;
static EFI_RUNTIME_SERVICES *runtime_services = NULL;
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
	runtime_services = system_table->RuntimeServices;

	// Turn off watchdog timer
	EFI_STATUS status = boot_services->SetWatchdogTimer(0, 0, 0, NULL);
	ASSERT_EFI_ERROR(status);

	// Get image handle for BrewBoot
	status = boot_services->HandleProtocol(image_handle, &gEfiLoadedImageProtocolGuid, (void **)&loaded_image);
	ASSERT_EFI_ERROR(status);

	// Load kernel
	status = load_kernel_image(L"Kernel.exe");
	if (EFI_ERROR(status))
	{
		return status;
	}

	kernel_main();

	boot_services->ExitBootServices(image_handle, 0);

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
	ASSERT_EFI_ERROR(status);

	// Get file handle for volume root
	EFI_FILE_PROTOCOL *volume = NULL;
	status = file_system->OpenVolume(file_system, &volume);
	ASSERT_EFI_ERROR(status);

	// Get file handle for kernel
	EFI_FILE_PROTOCOL *file = NULL;
	status = volume->Open(volume, &file, file_name, EFI_FILE_MODE_READ, EFI_FILE_READ_ONLY);
	if (status == EFI_NOT_FOUND)
	{
		Print(L"%s not found.\r\n", file_name);
		return status;
	}

	// Allocate buffer for file info
	UINTN file_info_size = 0x100;
	EFI_FILE_INFO *file_info = NULL;
	status = boot_services->AllocatePool(EfiBootServicesData, file_info_size, (void **)&file_info);
	ASSERT_EFI_ERROR(status);

	// Get file info
	status = file->GetInfo(file, &gEfiFileInfoGuid, &file_info_size, file_info);
	ASSERT_EFI_ERROR(status);

	UINTN file_size = file_info->FileSize;

	// Free file info buffer
	status = boot_services->FreePool(file_info);
	ASSERT_EFI_ERROR(status);

	// Allocate file read buffer
	void *file_buffer = NULL;
	status = boot_services->AllocatePool(EfiBootServicesData, file_size, (void **)&file_buffer);
	ASSERT_EFI_ERROR(status);

	// Read file into memory
	status = file->Read(file, &file_size, file_buffer);
	ASSERT_EFI_ERROR(status);

	PE_COFF_LOADER_IMAGE_CONTEXT image_context;
	ZeroMem(&image_context, sizeof image_context);

	image_context.Handle = file_buffer;
	image_context.ImageRead = PeCoffLoaderImageReadFromMemory;
	status = PeCoffLoaderGetImageInfo(&image_context);
	ASSERT_EFI_ERROR(status);

	void *image_buffer = NULL;
	status = boot_services->AllocatePool(EfiLoaderCode, image_context.ImageSize + image_context.SectionAlignment * 2, (void **)&image_buffer);
	ASSERT_EFI_ERROR(status);

	image_context.ImageAddress = (EFI_PHYSICAL_ADDRESS)image_buffer;
	image_context.ImageAddress += image_context.SectionAlignment - 1;
	image_context.ImageAddress &= ~((EFI_PHYSICAL_ADDRESS)image_context.SectionAlignment - 1);

	status = PeCoffLoaderLoadImage(&image_context);
	if (EFI_ERROR(status))
	{
		return status;
	}

	image_context.DestinationAddress = image_context.ImageAddress;
	PeCoffLoaderRelocateImage(&image_context);
	PeCoffLoaderRelocateImageExtraAction(&image_context);

	kernel_main = (func_ptr)image_context.EntryPoint;

	// Free file buffer
	status = boot_services->FreePool(file_buffer);
	ASSERT_EFI_ERROR(status);

	// Close file handles
	file->Close(file);
	volume->Close(volume);

	return EFI_SUCCESS;
}