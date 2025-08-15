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


// global variables for EFI services
EFI_SYSTEM_TABLE* gST = NULL;
EFI_BOOT_SERVICES* gBS = NULL;
EFI_RUNTIME_SERVICES* gRT = NULL;
EFI_HANDLE               gImageHandle = NULL;

// console output for debugging
EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL* ConOut = NULL;

/**
 * initialize global EFI service pointers
 */
EFI_STATUS
InitializeEfiServices(
    IN EFI_HANDLE ImageHandle,
    IN EFI_SYSTEM_TABLE* SystemTable
)
{
    // store global handles
    gImageHandle = ImageHandle;
    gST = SystemTable;
    gBS = SystemTable->BootServices;
    gRT = SystemTable->RuntimeServices;
    ConOut = SystemTable->ConOut;

    // clear screen and reset console
    ConOut->ClearScreen(ConOut);
    ConOut->Reset(ConOut, FALSE);

    return EFI_SUCCESS;
}

/**
 * print debug message to EFI console
 */
VOID
DebugPrint(
    IN CHAR16* Format,
    ...
)
{
    if (ConOut != NULL) {
        ConOut->OutputString(ConOut, Format);
    }
}

/**
 * initialize memory services and verify allocation works
 */
EFI_STATUS
InitializeMemoryServices(
    VOID
)
{
    EFI_STATUS Status;
    VOID* TestBuffer;

    DebugPrint(L"[+] Initializing memory services...\r\n");

    // test memory allocation
    Status = gBS->AllocatePool(
        EfiBootServicesData,
        1024,
        &TestBuffer
    );

    if (EFI_ERROR(Status)) {
        DebugPrint(L"[-] Failed to allocate test memory: %r\r\n", Status);
        return Status;
    }

    // clear test memory
    SetMem(TestBuffer, 1024, 0);

    // free test memory
    gBS->FreePool(TestBuffer);

    DebugPrint(L"[+] Memory services initialized successfully\r\n");
    return EFI_SUCCESS;
}

/**
 * initialize file system access
 */
EFI_STATUS
InitializeFileSystem(
    VOID
)
{
    EFI_STATUS Status;
    EFI_LOADED_IMAGE_PROTOCOL* LoadedImage;
    EFI_SIMPLE_FILE_SYSTEM_PROTOCOL* FileSystem;

    DebugPrint(L"[+] Initializing file system access...\r\n");

    // get loaded image protocol
    Status = gBS->HandleProtocol(
        gImageHandle,
        &gEfiLoadedImageProtocolGuid,
        (VOID**)&LoadedImage
    );

    if (EFI_ERROR(Status)) {
        DebugPrint(L"[-] Failed to get LoadedImage protocol: %r\r\n", Status);
        return Status;
    }

    // get file system protocol
    Status = gBS->HandleProtocol(
        LoadedImage->DeviceHandle,
        &gEfiSimpleFileSystemProtocolGuid,
        (VOID**)&FileSystem
    );

    if (EFI_ERROR(Status)) {
        DebugPrint(L"[-] Failed to get FileSystem protocol: %r\r\n", Status);
        return Status;
    }

    DebugPrint(L"[+] File system access initialized\r\n");
    return EFI_SUCCESS;
}

/**
 * main EFI application entry point
 */
EFI_STATUS
EFIAPI
UefiMain(
    IN EFI_HANDLE ImageHandle,
    IN EFI_SYSTEM_TABLE* SystemTable
)
{
    EFI_STATUS Status;
    EFI_INPUT_KEY Key;

    // initialize EFI services
    Status = InitializeEfiServices(ImageHandle, SystemTable);
    if (EFI_ERROR(Status)) {
        return Status;
    }

    // initialize memory services
    Status = InitializeMemoryServices();
    if (EFI_ERROR(Status)) {
        DebugPrint(L"[-] Memory initialization failed\r\n");
        goto Exit;
    }

    // initialize file system
    Status = InitializeFileSystem();
    if (EFI_ERROR(Status)) {
        DebugPrint(L"[-] File system initialization failed\r\n");
        goto Exit;
    }

    DebugPrint(L"[+] services sucess\r\n");
    DebugPrint(L"[+] EFI boilerplate ready for mapper \r\n");

Exit:
    DebugPrint(L"\r\nPress any key to continue boot\r\n");

    // Wait 
    gST->ConIn->Reset(gST->ConIn, FALSE);
    gST->ConIn->ReadKeyStroke(gST->ConIn, &Key);

    return Status;
}

    





