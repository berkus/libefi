#pragma once

// #define EFIAPI __attribute__((cdecl))  // 32 bits
#define EFIAPI __attribute__((ms_abi)) // 64 bits

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

typedef struct {
    UINT64 Signature;
    UINT32 Revision;
    UINT32 HeaderSize;
    UINT32 CRC32;
    UINT32 Reserved;
} EFI_TABLE_HEADER;

typedef struct
{
    EFI_TABLE_HEADER Hdr;
    CHAR16* FirmwareVendor;
    UINT32 FirmwareRevision;
    EFI_HANDLE ConsoleInHandle;
    EFI_SIMPLE_TEXT_INPUT_PROTOCOL* ConIn;
    EFI_HANDLE ConsoleOutHandle;
    EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL* ConOut;
    EFI_HANDLE StandardErrorHandle;
    EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL* StdErr;
    EFI_RUNTIME_SERVICES* RuntimeServices;
    EFI_BOOT_SERVICES* BootServices;
    UINTN NumberOfTableEntries;
    EFI_CONFIGURATION_TABLE* ConfigurationTable;
} EFI_SYSTEM_TABLE;

void InitializeLib(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable);
