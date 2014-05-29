// Simple EFI hello-world.
#include "efi/efi.h"
#include <wchar.h>

using namespace efi;

extern "C"
status_t
EFIAPI
_efi_main(handle_t ImageHandle, system_table_t *st)
{
    st->ConOut->output_string(L"\u25ba Welcome\r\n");
    st->ConOut->output_string(L"Hello EFI World\r\n");
    st->ConOut->output_string(L"Firmware vendor: ");
    st->ConOut->output_string(st->FirmwareVendor);
    st->ConOut->output_string(L"\r\n");

    if (st->is_valid_signature()) {
        st->ConOut->output_string(L"System table signature is valid\r\n");
    } else {
        st->ConOut->output_string(L"Invalid system table signature\r\n");
        return EFI_ERROR;
    }

    if (st->get_boot_services()->is_valid_signature()) {
        st->ConOut->output_string(L"Boot services table signature is valid\r\n");
    } else {
        st->ConOut->output_string(L"Invalid boot services table signature\r\n");
        return EFI_ERROR;
    }

    if (st->get_runtime_services().is_valid_signature()) {
        st->ConOut->output_string(L"Runtime services table signature is valid\r\n");
    } else {
        st->ConOut->output_string(L"Invalid runtime services table signature\r\n");
        return EFI_ERROR;
    }

    auto image = st->get_boot_services()->
        open_protocol<efi::loaded_image_protocol_t>(ImageHandle, ImageHandle,
            open_protocol_attribute::by_handle_protocol);

    if (!image) {
        st->ConOut->output_string(L"Error getting image protocol\r\n");
    } else {
        st->ConOut->output_string(L"Image protocol obtained successfully\r\n");
    }

    auto path = st->get_boot_services()->
        open_protocol<efi::loaded_image_device_path_protocol_t>(ImageHandle,
            ImageHandle, open_protocol_attribute::by_handle_protocol);

    if (!path) {
        st->ConOut->output_string(L"Error getting path protocol\r\n");
    } else {
        st->ConOut->output_string(L"Path protocol obtained successfully\r\n");
    }

    // wchar_t buf[256];
    // swprintf(buf, 256, L"%llu", image->ImageSize);

    // st->ConOut->output_string(L"Loaded image size: ");
    // st->ConOut->output_string(buf);
    // st->ConOut->output_string(std::to_wstring(image->ImageSize));
    // st->ConOut->output_string(L"\r\n");

    // st->get_runtime_services().shutdown();

    return EFI_SUCCESS;
}
