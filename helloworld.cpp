// Simple EFI hello-world.
#include "efi/efi.h"

extern "C"
EFI_STATUS
EFIAPI
efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable)
{
    (void)ImageHandle; // unused

    SystemTable->StdErr->OutputString(L"\u25ba Error logging\n\r");
    SystemTable->ConOut->OutputString(L"Hello EFI World\n\r");
    SystemTable->ConOut->OutputString(SystemTable->FirmwareVendor);
    SystemTable->ConOut->OutputString(L"\n\r");

    return EFI_SUCCESS;
}
