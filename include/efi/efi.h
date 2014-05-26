#pragma once

#include <stdint.h>

// #define EFIAPI __attribute__((cdecl))  // 32 bits
#define EFIAPI __attribute__((ms_abi)) // 64 bits

#define EFI_IN
#define EFI_OUT

typedef unsigned int EFI_STATUS;
typedef void* EFI_HANDLE;
typedef void* EFI_EVENT;
typedef char EFI_GUID[16];
typedef uint64_t EFI_LBA;
typedef unsigned int EFI_TPL;

enum { EFI_SUCCESS = 0, EFI_ERROR = 1 };

typedef EFI_HANDLE EFI_IMAGE;
typedef EFI_HANDLE EFI_RUNTIME_SERVICES; //temp
typedef EFI_HANDLE EFI_BOOT_SERVICES; //temp
typedef EFI_HANDLE EFI_CONFIGURATION_TABLE; //temp
typedef EFI_HANDLE EFI_SIMPLE_TEXT_INPUT_PROTOCOL; //temp

struct EFI_TABLE_HEADER
{
    uint64_t Signature;
    uint32_t Revision;
    uint32_t HeaderSize;
    uint32_t CRC32;
    uint32_t Reserved;
};

struct EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL
{
    EFI_STATUS (EFIAPI *_reset)(
        EFI_IN EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL* This,
        EFI_IN bool ExtendedVerification);
    EFI_STATUS (EFIAPI *_outputString)(
        EFI_IN EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL* This,
        EFI_IN wchar_t const* String);
    // EFI_TEXT_TEST_STRING TestString;
    // EFI_TEXT_QUERY_MODE QueryMode;
    // EFI_TEXT_SET_MODE SetMode;
    // EFI_TEXT_SET_ATTRIBUTE SetAttribute;
    // EFI_TEXT_CLEAR_SCREEN ClearScreen;
    // EFI_TEXT_SET_CURSOR_POSITION SetCursorPosition;
    // EFI_TEXT_ENABLE_CURSOR EnableCursor;
    // SIMPLE_TEXT_OUTPUT_MODE* Mode;

    EFI_STATUS Reset(bool ExtendedVerification = false) {
        return _reset(this, ExtendedVerification);
    }
    EFI_STATUS OutputString(wchar_t const* String) {
        return _outputString(this, String);
    }
};

struct EFI_SYSTEM_TABLE
{
    EFI_TABLE_HEADER                 Hdr;
    wchar_t*                         FirmwareVendor;
    uint32_t                         FirmwareRevision;
    EFI_HANDLE                       ConsoleInHandle;
    EFI_SIMPLE_TEXT_INPUT_PROTOCOL*  ConIn;
    EFI_HANDLE                       ConsoleOutHandle;
    EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL* ConOut;
    EFI_HANDLE                       StandardErrorHandle;
    EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL* StdErr;
    EFI_RUNTIME_SERVICES*            RuntimeServices;
    EFI_BOOT_SERVICES*               BootServices;
    unsigned int                     NumberOfTableEntries;
    EFI_CONFIGURATION_TABLE*         ConfigurationTable;
};

void InitializeLib(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable);
