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

namespace efi {

// Calling convention specifier for UEFI-callable functions.
// ms_abi works for both 32 and 64 bit, since on 32 bits system is it equivalent of required cdecl.
#define EFIAPI __attribute__((ms_abi))

// Documentation markers for which arguments in UEFI-callable functions are IN and which are OUT.
#define EFI_IN
#define EFI_OUT

//=================================================================================================
// EFI defined types
//=================================================================================================

typedef unsigned int status_t;
typedef void*        handle_t;
typedef handle_t     image_t;
typedef void*        EFI_EVENT;
typedef char         guid_t[16];
typedef uint64_t     EFI_LBA;
typedef unsigned int EFI_TPL;
typedef uint64_t     physical_address_t;
typedef uint64_t     virtual_address_t;

/**
 * Return error codes. @todo Complete!
 */
enum : status_t {
    EFI_SUCCESS = 0,
    EFI_ERROR = 1
};

typedef handle_t EFI_SIMPLE_TEXT_INPUT_PROTOCOL; //temp

//=================================================================================================
// EFI defined protocols
//
// In the abstract, a protocol consists of a 128-bit globally unique identifier (GUID)
// and a Protocol Interface structure. The structure contains the functions and instance
// data that are used to access a device. The functions that make up Protocol Handler
// Services allow applications to install a protocol on a handle, identify the handles
// that support a given protocol, determine whether a handle supports a given protocol,
// and so forth. (excerpt from UEFI spec)
//=================================================================================================


class simple_text_output_protocol_t
{
    status_t (EFIAPI *_Reset)(
        EFI_IN simple_text_output_protocol_t* This,
        EFI_IN bool ExtendedVerification);
    status_t (EFIAPI *_OutputString)(
        EFI_IN simple_text_output_protocol_t* This,
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
    status_t reset(bool extended_verification = false) {
        return _Reset(this, extended_verification);
    }
    status_t output_string(wchar_t const* string) {
        return _OutputString(this, string);
    }
};

struct memory_descriptor_t
{
    uint32_t           Type;
    physical_address_t PhysicalStart;
    virtual_address_t  VirtualStart;
    uint64_t           NumberOfPages;
    uint64_t           Attribute;
};

//=================================================================================================
// EFI defined tables
//=================================================================================================

/**
 * Shared table header.
 */
struct table_header_t
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
class boot_services_t : public table_header_t
{
    static const uint64_t SIGNATURE = 0x56524553544f4f42;

    //
    // Task Priority services
    //
    void* _RaiseTPL; // EFI 1.0+
    void* _RestoreTPL; // EFI 1.0+
    //
    // Memory services
    //
    void* _AllocatePages; // EFI 1.0+
    void* _FreePages; // EFI 1.0+
    status_t (EFIAPI* _GetMemoryMap)(
        EFI_IN EFI_OUT unsigned int* MemoryMapSize,
        EFI_IN EFI_OUT memory_descriptor_t* MemoryMap,
        EFI_OUT unsigned int* MapKey,
        EFI_OUT unsigned int* DescriptorSize,
        EFI_OUT uint32_t* DescriptorVersion); // EFI 1.0+
    void* _AllocatePool; // EFI 1.0+
    void* _FreePool; // EFI 1.0+
    //
    // Event and Timer services
    //
    void *_CreateEvent; // EFI 1.0+
    void *_SetTimer; // EFI 1.0+
    void *_WaitForEvent; // EFI 1.0+
    void *_SignalEvent; // EFI 1.0+
    void *_CloseEvent; // EFI 1.0+
    void *_CheckEvent; // EFI 1.0+
    //
    // Protocol Handler services
    //
    void* _InstallProtocolInterface; // EFI 1.0+
    void* _ReinstallProtocolInterface; // EFI 1.0+
    void* _UninstallProtocolInterface; // EFI 1.0+
    void* _HandleProtocol; // EFI 1.0+
    void* _Reserved; // EFI 1.0+
    void* _RegisterProtocolNotify; // EFI 1.0+
    void* _LocateHandle; // EFI 1.0+
    void* _LocateDevicePath; // EFI 1.0+
    void* _InstallConfigurationTable; // EFI 1.0+
    //
    // Image services
    //
    void* _LoadImage; // EFI 1.0+
    void* _StartImage; // EFI 1.0+
    void* _Exit; // EFI 1.0+
    void* _UnloadImage; // EFI 1.0+
    void* _ExitBootServices; // EFI 1.0+
    //
    // Miscellaneous services
    //
    void* _GetNextMonotonicCount; // EFI 1.0+
    void* _Stall; // EFI 1.0+
    void* _SetWatchdogTimer; // EFI 1.0+
    //
    // Driver Support services
    //
    void* _ConnectController; // EFI 1.1+
    void* _DisconnectController; // EFI 1.1+
    //
    // Open And Close Protocol services
    //
    void* _OpenProtocol; // EFI 1.1+
    void* _CloseProtocol; // EFI 1.1+
    void* _OpenProtocolInformation; // EFI 1.1+
    //
    // Library services
    //
    void* _ProtocolsPerHandle; // EFI 1.1+
    void* _LocateHandleBuffer; // EFI 1.1+
    void* _LocateProtocol; // EFI 1.1+
    void* _InstallMultipleProtocolInterfaces; // EFI 1.1+
    void* _UninstallMultipleProtocolInterfaces; // EFI 1.1+
    //
    // 32-bit CRC services
    //
    void* _CalculateCrc32; // EFI 1.1+
    //
    // Miscellaneous services
    //
    void* _CopyMem; // EFI 1.1+
    void* _SetMem; // EFI 1.1+
    void* _CreateEventEx; // UEFI 2.0+

public:
    inline bool is_valid_signature() const { return Signature == SIGNATURE; }
};

struct runtime_services_t : public table_header_t
{
    static const uint64_t SIGNATURE = 0x56524553544e5552;

    //
    // Time services
    //
    void* _GetTime;
    void* _SetTime;
    void* _GetWakeupTime;
    void* _SetWakeupTime;
    //
    // Virtual Memory services
    //
    void* _SetVirtualAddressMap;
    void* _ConvertPointer;
    //
    // Variable services
    //
    void* _GetVariable;
    void* _GetNextVariableName;
    void* _SetVariable;
    //
    // Miscellaneous services
    //
    void* _GetNextHighMonotonicCount;
    void* _ResetSystem;
    //
    // UEFI 2.0 Capsule services
    //
    void* _UpdateCapsule;
    void* _QueryCapsuleCapabilities;
    //
    // Miscellaneous UEFI 2.0 services
    //
    void* _QueryVariableInfo;

public:
    inline bool is_valid_signature() const { return Signature == SIGNATURE; }
};

/**
 * Single entry in the ConfigurationTable array below.
 */
struct configuration_table_t
{
    guid_t VendorGuid;
    void*  VendorTable;
};

/**
 * System table passed to main function in EFI Applications, Drivers and OS Loaders alike.
 */
struct system_table_t : public table_header_t
{
    static const uint64_t SIGNATURE = 0x5453595320494249;

    wchar_t*                         FirmwareVendor;
    uint32_t                         FirmwareRevision;
    handle_t                         ConsoleInHandle;
    EFI_SIMPLE_TEXT_INPUT_PROTOCOL*  ConIn;
    handle_t                         ConsoleOutHandle;
    simple_text_output_protocol_t*   ConOut;
    handle_t                         StandardErrorHandle;
    simple_text_output_protocol_t*   StdErr;
    runtime_services_t*              RuntimeServices;
    boot_services_t*                 BootServices;
    unsigned int                     NumberOfTableEntries;
    configuration_table_t*           ConfigurationTable;

public:
    inline bool is_valid_signature() const { return Signature == SIGNATURE; }
};

void InitializeLib(handle_t ImageHandle, system_table_t *systemTable);

} // efi namespace
