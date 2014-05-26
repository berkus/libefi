#include "efi.h"

// Pointers to the various global UEFI functions are found in the EFI_RUNTIME_SERVICES
// and EFI_BOOT_SERVICES tables that are located via the system table. Pointers to other
// functions defined in this specification are located dynamically through device
// handles. In all cases, all pointers to UEFI functions are cast with the word EFIAPI

void
InitializeLib(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable)
{}
