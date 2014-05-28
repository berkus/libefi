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
#define EFI_IN             // Function input argument.
#define EFI_OUT            // Function output argument.
#define EFI_OPTIONAL       // Function optional argument, can be supplied as nullptr.

//=================================================================================================
// EFI defined types
//=================================================================================================

typedef unsigned int  status_t;
typedef void*         handle_t;
typedef handle_t      image_t;
typedef void*         event_t;
typedef unsigned char guid_t[16];
typedef uint64_t      lba_t;
typedef unsigned int  tpl_t;
typedef uint64_t      physical_address_t;
typedef uint64_t      virtual_address_t;

/**
 * Return error codes. @todo Complete!
 */
enum : status_t {
    EFI_SUCCESS = 0,
    EFI_ERROR = 1
};

#define EFI_ERROR(e) (e != EFI_SUCCESS)

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
        EFI_IN uint8_t ExtendedVerification);
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
    static constexpr guid_t guid = {0x0};

    status_t reset(bool extended_verification = false) {
        return _Reset(this, extended_verification);
    }
    status_t output_string(wchar_t const* string) {
        return _OutputString(this, string);
    }
};

//=================================================================================================
// EFI defined supplemental types
//=================================================================================================

struct time_t
{
    uint16_t Year;         // 1900 - 9999
    uint8_t  Month;        // 1 - 12
    uint8_t  Day;          // 1 - 31
    uint8_t  Hour;         // 0 - 23
    uint8_t  Minute;       // 0 - 59
    uint8_t  Second;       // 0 - 59
    uint8_t  Pad1;
    uint32_t Nanosecond;   // 0 - 999,999,999
    int16_t  TimeZone;     // -1440 - 1440 or 2047
    uint8_t  Daylight;
    uint8_t  Pad2;
};

struct time_capabilities_t
{
    uint32_t Resolution;
    uint32_t Accuracy;
    uint8_t  SetsToZero;
};

/**
 * Descriptor for virtual memory and memory map operations.
 */
struct memory_descriptor_t
{
    uint32_t           Type;
    physical_address_t PhysicalStart;
    virtual_address_t  VirtualStart;
    uint64_t           NumberOfPages;
    uint64_t           Attribute;
};

enum reset_type : unsigned int
{
    cold,
    warm,
    shutdown,
    platform_specific
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
    static constexpr uint64_t SIGNATURE = 0x56524553544f4f42;

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
    status_t (EFIAPI *_GetMemoryMap)(
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
    /// Queries a handle to determine if it supports a specified protocol.
    /// *DEPRECATED*
    /// All new applications and drivers should use OpenProtocol() in place of HandleProtocol().
    status_t (EFIAPI *_HandleProtocol)(
        EFI_IN handle_t Handle,
        EFI_IN guid_t* Guid,
        EFI_OUT void** Interface); // EFI 1.0+
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
    /// Terminates a loaded EFI image and returns control to boot services.
    status_t (EFIAPI *_Exit)(
        EFI_IN              handle_t     ImageHandle,
        EFI_IN              status_t     ExitStatus,
        EFI_IN              unsigned int ExitDataSize,
        EFI_IN EFI_OPTIONAL wchar_t*     ExitData); // EFI 1.0+
    void* _UnloadImage; // EFI 1.0+
    /// Terminates all boot services.
    status_t (EFIAPI *_ExitBootServices)(
        EFI_IN handle_t     ImageHandle,
        EFI_IN unsigned int MapKey); // EFI 1.0+
    //
    // Miscellaneous services
    //
    void* _GetNextMonotonicCount; // EFI 1.0+
    void* _Stall; // EFI 1.0+
    /// Sets the system’s watchdog timer.
    status_t (EFIAPI *_SetWatchdogTimer)(
        EFI_IN              unsigned int Timeout,
        EFI_IN              uint64_t     WatchdogCode,
        EFI_IN              unsigned int DataSize,
        EFI_IN EFI_OPTIONAL wchar_t*     WatchdogData); // EFI 1.0+
    //
    // Driver Support services
    //
    void* _ConnectController; // EFI 1.1+
    void* _DisconnectController; // EFI 1.1+
    //
    // Open And Close Protocol services
    //
    /// Queries a handle to determine if it supports a specified protocol.
    /// If the protocol is supported by the handle, it opens the protocol
    /// on behalf of the calling agent.
    status_t (EFIAPI *_OpenProtocol)(
        EFI_IN               handle_t Handle,
        EFI_IN               guid_t*  Protocol,
        EFI_OUT EFI_OPTIONAL void**   Interface,
        EFI_IN               handle_t AgentHandle,
        EFI_IN EFI_OPTIONAL  handle_t ControllerHandle,
        EFI_IN               uint32_t Attributes); // EFI 1.1+
    /// Closes a protocol on a handle that was opened using OpenProtocol().
    status_t (EFIAPI *_CloseProtocol)(
        EFI_IN               handle_t Handle,
        EFI_IN               guid_t*  Protocol,
        EFI_IN               handle_t AgentHandle,
        EFI_IN EFI_OPTIONAL  handle_t ControllerHandle); // EFI 1.1+
    /// Retrieves the list of agents that currently have a protocol interface opened.
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
    /// Computes and returns a 32-bit CRC for a data buffer.
    status_t (EFIAPI *_CalculateCrc32)(
        EFI_IN  void*        Data,
        EFI_IN  unsigned int DataSize,
        EFI_OUT uint32_t*    Crc32); // EFI 1.1+
    //
    // Miscellaneous services
    //
    /// Copies the contents of one buffer to another buffer.
    void (EFIAPI *_CopyMem)(
        EFI_IN void*        Destination,
        EFI_IN void*        Source,
        EFI_IN unsigned int Size); // EFI 1.1+
    /// Fills a buffer with a specified value.
    void (EFIAPI *_SetMem)(
        EFI_IN void*        Buffer,
        EFI_IN unsigned int Size,
        EFI_IN uint8_t      Value); // EFI 1.1+
    void* _CreateEventEx; // UEFI 2.0+

public:
    inline bool is_valid_signature() const { return Signature == SIGNATURE; }
};

/**
 * Runtime services are available during all the time under UEFI firmware.
 */
struct runtime_services_t : public table_header_t
{
    static constexpr uint64_t SIGNATURE = 0x56524553544e5552;

    //
    // Time services
    //
    /// Returns the current time and date information,
    /// and the time-keeping capabilities of the hardware platform.
    status_t (EFIAPI *_GetTime)(
        EFI_OUT              time_t*              Time,
        EFI_OUT EFI_OPTIONAL time_capabilities_t* Capabilities);
    /// Sets the current local time and date information.
    status_t (EFIAPI *_SetTime)(
        EFI_IN time_t* Time);
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
    /// Enumerates the current variable names.
    status_t (EFIAPI *_GetNextVariableName)(
        EFI_IN EFI_OUT unsigned int* VariableNameSize,
        EFI_IN EFI_OUT wchar_t* VariableName,
        EFI_IN EFI_OUT guid_t* VendorGuid);
    void* _SetVariable;
    //
    // Miscellaneous services
    //
    void* _GetNextHighMonotonicCount;
    /// Resets the entire platform.
    void (EFIAPI *_ResetSystem)(
        EFI_IN reset_type         ResetType,
        EFI_IN status_t           ResetStatus,
        EFI_IN unsigned int       DataSize,
        EFI_IN EFI_OPTIONAL void* ResetData);
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

    void reboot()   { _ResetSystem(reset_type::warm, EFI_SUCCESS, 0, nullptr); }
    void shutdown() { _ResetSystem(reset_type::shutdown, EFI_SUCCESS, 0, nullptr); }
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
    static constexpr uint64_t SIGNATURE = 0x5453595320494249;

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

    runtime_services_t& get_runtime() { return *RuntimeServices; }
};

} // efi namespace
