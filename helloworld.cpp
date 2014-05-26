// Simple EFI hello-world.
#include "efi/efi.h"

EFI_STATUS
EFIAPI
efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable)
{
    (void)ImageHandle; // unused

    SystemTable->ConOut->OutputString(L"Hello World\n\r");

    return EFI_SUCCESS;
}
