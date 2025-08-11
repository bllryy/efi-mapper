/*
file handles:
- EFI application entry point
- Basic EFI services initialization
- Memory allocation setup
- File system access preparation
- Error handling framework
*/

/*
EFI_STATUS EFIAPI efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable);
- initialize EFI services
- set up console output (debugging)
- initialize memory services
- prepare file system access
- basic error handling
*/


#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Protocol/SimpleFileSystem.h>
#include <Protocol/LoadedImage.h>


// vars for EFI services
EFI_SYSTEM_TABLE* gST = NULL;
EFI_BOOT_SERVICES* gBS = NULL;
EFI_RUNTIME_SERVICES* gRT = NULL;
EFI_HANDLE               gImageHandle = NULL;

// output for debug
EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL* ConOut = NULL;



// Start the global EFI service pointers
EFI_STATUS
InitializeEfiServices(
	IN EFI_HANDLE ImageHandle,
	IN EFI_SYSTEM_TABLE* SystemTable
)
{
// store the global handles
    gImageHandle = ImageHandle;
    gST = SystemTable;
    gBS = SystemTable->BootServices;
    gRT = SystemTable->RuntimeServices;
    ConOut = SystemTable->ConOut;

    ConOut->ClearScreen(ConOut);
    ConOut->Reset(ConOut, FALSE);

    return EFI_SUCCESS;
}





