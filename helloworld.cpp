// Simple EFI hello-world.
#include "efi/efi.h"

using namespace efi;

extern "C"
status_t
EFIAPI
efi_main(handle_t ImageHandle, system_table_t *st)
{
    (void)ImageHandle; // unused

    st->ConOut->output_string(L"\u25ba Welcome\n\r");
    st->ConOut->output_string(L"Hello EFI World\n\r");
    st->ConOut->output_string(st->FirmwareVendor);
    st->ConOut->output_string(L"\n\r");

    st->get_runtime().shutdown();

    return EFI_SUCCESS;
}
