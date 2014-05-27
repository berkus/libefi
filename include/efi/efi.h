//
// Part of Metta OS. Check http://atta-metta.net for latest version.
//
// Copyright 2007 - 2014, Stanislav Karchebnyy <berkus@exquance.com>
//
// Distributed under the Boost Software License, Version 1.0.
// (See file LICENSE_1_0.txt or a copy at http://www.boost.org/LICENSE_1_0.txt)
//
#pragma once

#include <stdint.h>

// Calling convention specifier for UEFI-callable functions.
// ms_abi works for both 32 and 64 bit, since on 32 bits system is it equivalent of required cdecl.
#define EFIAPI __attribute__((ms_abi))

// Documentation markers for which arguments in UEFI-callable functions are IN and which are OUT.
#define EFI_IN
#define EFI_OUT

//=================================================================================================
// EFI defined types
//=================================================================================================

typedef unsigned int EFI_STATUS;
typedef void*        EFI_HANDLE;
typedef EFI_HANDLE   EFI_IMAGE;
typedef void*        EFI_EVENT;
typedef char         EFI_GUID[16];
typedef uint64_t     EFI_LBA;
typedef unsigned int EFI_TPL;

enum : EFI_STATUS {
    EFI_SUCCESS = 0,
    EFI_ERROR = 1
};

typedef EFI_HANDLE EFI_CONFIGURATION_TABLE; //temp
typedef EFI_HANDLE EFI_SIMPLE_TEXT_INPUT_PROTOCOL; //temp

class EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL
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

public:
    EFI_STATUS Reset(bool ExtendedVerification = false) {
        return _reset(this, ExtendedVerification);
    }
    EFI_STATUS OutputString(wchar_t const* String) {
        return _outputString(this, String);
    }
};

//=================================================================================================
// EFI defined tables
//=================================================================================================

/**
 * Shared table header.
 */
struct EFI_TABLE_HEADER
{
    uint64_t Signature;
    uint32_t Revision;
    uint32_t HeaderSize;
    uint32_t CRC32;
    uint32_t Reserved;
};

/**
 * Services used by OS Loader in pre-boot environment.
 * Use ExitBootServices() to free this table and continue booting an OS.
 */
struct EFI_BOOT_SERVICES
{
    EFI_TABLE_HEADER                 Hdr;
};

struct EFI_RUNTIME_SERVICES
{
    EFI_TABLE_HEADER                 Hdr;
};

/**
 * System table passed to main function in EFI Applications, Drivers and OS Loaders alike.
 */
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
