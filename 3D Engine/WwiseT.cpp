#include "WwiseT.h"

// Wwise stuff
#ifdef WIN32
void * AK::AllocHook(size_t in_size)
{
	return malloc(in_size);
}

void AK::FreeHook(void * in_ptr)
{
	free(in_ptr);
}

void * AK::VirtualAllocHook(void * in_pMemAddress, size_t in_size, DWORD in_dwAllocationType, DWORD in_dwProtect)
{
	return VirtualAlloc(in_pMemAddress, in_size, in_dwAllocationType, in_dwProtect);
}

void AK::VirtualFreeHook(void * in_pMemAddress, size_t in_size, DWORD in_dwFreeType)
{
	VirtualFree(in_pMemAddress, in_size, in_dwFreeType);
}
#endif

