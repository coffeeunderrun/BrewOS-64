#ifndef PE_H
#define PE_H

#include <Uefi.h>

typedef void(*PE_ENTRY_POINT)();

EFI_STATUS EFIAPI load_pe_image(IN EFI_BOOT_SERVICES *boot_services, IN EFI_HANDLE handle, IN CHAR16 *file_name, OUT PE_ENTRY_POINT *entry_point);

#endif