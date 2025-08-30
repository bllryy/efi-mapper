/*
implements the actual memory mapping logic that loads a driver into memory without going through the normal Windows driver loading process

also need a header file for function declarations and structure definitions
*/


#include "util.h"
#include "mmap.h"

// external refrences to global EFI services 

extern EFI_BOOT_SERVICES* gBS;
extern EFI_SYSTEM_TABLE* gST;
extern EFI_RUNTIME_SERVICES* gRT;


/*
Allocate memory for the mapped image 
*/

VOID*
AllocateImageMemory(
	IN UINTN Size
)
{
	EFI_STATUS Status;
	EFI_PHYSICAL_ADDRESS Address = 0;
	UINTN Pages;

	// compute the number of pages needed
	Pages = EFI_SIZE_TO_PAGES(ImageSize);

	// allocate the memory
	Status = gBS->AllocatePages(
		AllocateAnyPages,
		EfiBootServicesCode, // use code memory type
		Pages,
		&Address
	);

	if(EFI_ERROR(Status))
	{
		Print(L"Failed to allocate pages for image memory: %r\n", Status);
		return NULL;
	}

	PrintDebug(L"[-] Allocated %d bytes at 0x%p\r\n", ImageSize, (VOID*)Addresses);
	return (VOID*)Addresses;
}

/*
* copy the PE sec tions into the allocated memory
*/

EFI_STATUS
MapPESections(
	IN VOID* SourceImage,
	IN VOID* DestImage,
	IN PIMAGE_NT_HEADERS NtHeders
)
{
	PIMAGE_SECTION_HEADER SectionHeader;
	UINT16 i;

	PrintDebug(L"[-] Mapping PE sections...\r\n");

	// get the first section header
	SectionHeader = GetFirstSectionHeader(NtHeaders);

	// copy each section
	for (i = 0; i < NtHeaders->FileHeader.NumberOfSections; i++)
	{
		VOID *SectionDest;
		VOID *SectionSrc;
		UINTN CopySize;

		// compute source and destination addresses
		Src = (VOID*)((UINTN)SourceImage + SectionHeader[i].PointerToRawData);
		Dst = (VOID*)((UINTN)DestImage + SectionHeader[i].VirtualAddress);

		// copy the section data
		MemCopy(Dst, Src, SectionHeader[i].SizeOfRawData);
		PrintDebug(L"    - Mapped section %a to 0x%p\r\n", SectionHeader[i].Name, Dst);
	}

	// TODO : handle relocations and imports
}