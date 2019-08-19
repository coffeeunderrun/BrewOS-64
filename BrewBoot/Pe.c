#include <Uefi.h>
#include <Guid/FileInfo.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/PeCoffLib.h>
#include <Library/UefiLib.h>
#include <Protocol/SimpleFileSystem.h>
#include <Uefi/UefiSpec.h>
#include "Pe.h"

/*
 * load_pe_image
 */
EFI_STATUS EFIAPI load_pe_image(IN EFI_BOOT_SERVICES *boot_services, IN EFI_HANDLE handle, IN CHAR16 *file_name, OUT PE_ENTRY_POINT *entry_point)
{
	ASSERT(boot_services != NULL);
	ASSERT(file_name != NULL);

	// Get file system
	EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *file_system = NULL;
	EFI_STATUS status = boot_services->HandleProtocol(handle, &gEfiSimpleFileSystemProtocolGuid, (void **)&file_system);
	if (EFI_ERROR(status))
	{
		DEBUG((EFI_D_ERROR, "\nFailed to get file system. (%r)\r\n", status));
		return status;
	}

	// Get file handle for volume root
	EFI_FILE_PROTOCOL *volume = NULL;
	status = file_system->OpenVolume(file_system, &volume);
	if (EFI_ERROR(status))
	{
		DEBUG((EFI_D_ERROR, "\nFailed to open boot volume. (%r)\r\n", status));
		return status;
	}

	// Get file handle for kernel
	EFI_FILE_PROTOCOL *file = NULL;
	status = volume->Open(volume, &file, file_name, EFI_FILE_MODE_READ, EFI_FILE_READ_ONLY);
	if (EFI_ERROR(status))
	{
		DEBUG((EFI_D_ERROR, "\nFailed to load kernel '%s'. (%r)\r\n", file_name, status));
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
		DEBUG((EFI_D_ERROR, "\nFailed to allocate file info buffer. (%r)\r\n", status));
		return status;
	}

	// Get file info
	status = file->GetInfo(file, &gEfiFileInfoGuid, &file_info_size, file_info);
	if (EFI_ERROR(status))
	{
		DEBUG((EFI_D_ERROR, "\nFailed to get file info for kernel. (%r)\r\n", status));
		return status;
	}

	UINTN file_size = file_info->FileSize;

	// Free file info buffer
	status = boot_services->FreePool(file_info);
	if (EFI_ERROR(status))
	{
		DEBUG((EFI_D_ERROR, "\nFailed to dispose of file info buffer. (%r)\r\n", status));
		return status;
	}

	// Allocate file read buffer
	void *file_buffer = NULL;
	status = boot_services->AllocatePool(EfiBootServicesData, file_size, (void **)&file_buffer);
	if (EFI_ERROR(status))
	{
		DEBUG((EFI_D_ERROR, "\nFailed to allocate file read buffer. (%r)\r\n", status));
		return status;
	}

	// Read file into memory
	status = file->Read(file, &file_size, file_buffer);
	if (EFI_ERROR(status))
	{
		DEBUG((EFI_D_ERROR, "\nFailed to read kernel into file read buffer. (%r)\r\n", status));
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
		DEBUG((EFI_D_ERROR, "\nFailed to get image info. (%r)\r\n", status));
		return status;
	}

	// Allocate image buffer
	void *image_buffer = NULL;
	status = boot_services->AllocatePool(EfiBootServicesCode, image_context.ImageSize + image_context.SectionAlignment * 2, (void **)&image_buffer);
	if (EFI_ERROR(status))
	{
		DEBUG((EFI_D_ERROR, "\nFailed to allocate image buffer. (%r)\r\n", status));
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
		DEBUG((EFI_D_ERROR, "\nFailed to load kernel into image buffer. (%r)\r\n", status));
		return status;
	}

	// Apply relocation to image
	image_context.DestinationAddress = image_context.ImageAddress;
	status = PeCoffLoaderRelocateImage(&image_context);
	if (EFI_ERROR(status))
	{
		DEBUG((EFI_D_ERROR, "\nFailed to apply relocation to image. (%r)\r\n", status));
		return status;
	}

	// Save image entry point
	*entry_point = (PE_ENTRY_POINT)image_context.EntryPoint;

	// Dispose of file read buffer
	status = boot_services->FreePool(file_buffer);
	if (EFI_ERROR(status))
	{
		DEBUG((EFI_D_ERROR, "\nFailed to dispose of file read buffer. (%r)\r\n", status));
		return status;
	}

	// Close file handle
	status = file->Close(file);
	if (EFI_ERROR(status))
	{
		DEBUG((EFI_D_ERROR, "\nFailed to close file handle. (%r)\r\n", status));
		return status;
	}

	// Close volume handle
	status = volume->Close(volume);
	if (EFI_ERROR(status))
	{
		DEBUG((EFI_D_ERROR, "\nFailed to close boot volume. (%r)\r\n", status));
		return status;
	}

	return EFI_SUCCESS;
}
