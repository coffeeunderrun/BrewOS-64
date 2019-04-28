#include <Uefi.h>
#include <Uefi/UefiSpec.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PeCoffLib.h>
#include <Protocol/LoadedImage.h>
#include <Protocol/SimpleFileSystem.h>

CHAR8 *gEfiCallerBaseName = "BrewBoot";

EFI_STATUS EFIAPI brew_boot_main(IN EFI_HANDLE image_handle, IN EFI_SYSTEM_TABLE *system_table)
{
	ASSERT(image_handle != NULL);
	ASSERT(system_table != NULL);

	system_table->BootServices->SetWatchdogTimer(0, 0, 0, NULL);

	PE_COFF_LOADER_IMAGE_CONTEXT kernel_image_context;
	ZeroMem(&kernel_image_context, sizeof kernel_image_context);

	EFI_LOADED_IMAGE_PROTOCOL *loaded_image = NULL;
	EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *file_system = NULL;
	EFI_FILE_PROTOCOL *volume_root = NULL;
	EFI_FILE_PROTOCOL *kernel_file = NULL;

	EFI_MEMORY_DESCRIPTOR *descriptor = NULL;
	UINTN map_key = 0, map_size = 0, descriptor_size = 0;
	UINT32 descriptor_version;

	EFI_STATUS status = system_table->BootServices->HandleProtocol(image_handle, &gEfiLoadedImageProtocolGuid, (void **)&loaded_image);
	ASSERT_EFI_ERROR(status);

	status = system_table->BootServices->HandleProtocol(loaded_image->DeviceHandle, &gEfiSimpleFileSystemProtocolGuid, (void **)&file_system);
	ASSERT_EFI_ERROR(status);

	status = file_system->OpenVolume(file_system, &volume_root);
	ASSERT_EFI_ERROR(status);

	status = volume_root->Open(volume_root, &kernel_file, L"kernel.dll", EFI_FILE_MODE_READ, 0);
	ASSERT_EFI_ERROR(status);

	kernel_image_context.Handle = kernel_file;
	kernel_image_context.ImageRead = PeCoffLoaderImageReadFromMemory;
	status = PeCoffLoaderGetImageInfo(&kernel_image_context);
	ASSERT_EFI_ERROR(status);

	void *buffer = AllocatePages(EFI_SIZE_TO_PAGES(kernel_image_context.ImageSize));
	ASSERT(buffer != NULL);

	kernel_image_context.ImageAddress = (EFI_PHYSICAL_ADDRESS)(UINTN)buffer;
	status = PeCoffLoaderLoadImage(&kernel_image_context);
	ASSERT_EFI_ERROR(status);

	kernel_file->Close(kernel_file);
	volume_root->Close(volume_root);

	status = system_table->BootServices->GetMemoryMap(&map_size, descriptor, &map_key, &descriptor_size, &descriptor_version);
	ASSERT_EFI_ERROR(status);

	status = system_table->BootServices->ExitBootServices(image_handle, map_key);
	ASSERT_EFI_ERROR(status);

	return EFI_SUCCESS;
}