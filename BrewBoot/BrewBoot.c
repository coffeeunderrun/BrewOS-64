#include <Uefi.h>
#include <Library/DebugLib.h>
#include <Protocol/LoadedImage.h>
#include <Uefi/UefiSpec.h>
#include "Pe.h"

#ifdef IS_DEBUG
extern void kernel_main();
#endif

/*
 * brew_boot_main
 */
EFI_STATUS EFIAPI brew_boot_main(IN EFI_HANDLE image_handle, IN EFI_SYSTEM_TABLE *system_table)
{
	ASSERT(image_handle != NULL);
	ASSERT(system_table != NULL);

	// Turn off watchdog timer
	EFI_STATUS status = system_table->BootServices->SetWatchdogTimer(0, 0, 0, NULL);
	if (EFI_ERROR(status))
	{
		DEBUG((EFI_D_ERROR, "\nFailed to disable watchdog timer. (%r)\r\n", status));
	}

	system_table->ConOut->EnableCursor(system_table->ConOut, FALSE);
	system_table->ConOut->ClearScreen(system_table->ConOut);
	system_table->ConOut->OutputString(system_table->ConOut, L"BrewOS x64 v0.1\r\n");

	// Get image handle for BrewBoot
	EFI_LOADED_IMAGE_PROTOCOL *loaded_image;
	status = system_table->BootServices->HandleProtocol(image_handle, &gEfiLoadedImageProtocolGuid, (void **)&loaded_image);
	if (EFI_ERROR(status))
	{
		DEBUG((EFI_D_ERROR, "\nFailed to boot image handle. (%r)\r\n", status));
		return status;
	}

	// Load kernel
	PE_ENTRY_POINT kernel_entry_point;
#ifdef IS_DEBUG
	// When debugging, the kernel is linked statically so the entry point is already known
	kernel_entry_point = &kernel_main;
#else
	// When not debugging, load the kernel and find the entry point
	status = load_pe_image(system_table->BootServices, loaded_image->DeviceHandle, L"Kernel.dll", &kernel_entry_point);
	if (EFI_ERROR(status))
	{
		return status;
	}
#endif

	// Allocate buffer for memory map
	UINTN map_key = 0;
	UINTN memory_map_size = SIZE_32KB;
	EFI_MEMORY_DESCRIPTOR *memory_map;
	status = system_table->BootServices->AllocatePool(EfiBootServicesData, memory_map_size, (void **)&memory_map);
	ASSERT_EFI_ERROR(status);

	// Get memory map
	UINTN descriptor_size = 0;
	UINT32 descriptor_version = 0;
	status = system_table->BootServices->GetMemoryMap(&memory_map_size, memory_map, &map_key, &descriptor_size, &descriptor_version);
	ASSERT_EFI_ERROR(status);

	// Don't need boot services any more
	status = system_table->BootServices->ExitBootServices(image_handle, map_key);
	if (EFI_ERROR(status))
	{
		DEBUG((EFI_D_ERROR, "\nFailed to exit boot services. (%r)\r\n", status));
	}

	// Jump to kernel
	kernel_entry_point();

	// Shouldn't get here since the kernel should handle restart and shutdown
	return EFI_SUCCESS;
}
