#ifndef WWISET_H_
#define WWISET_H_

#include "Globals.h"
// Wwise libs
#ifndef _DEBUG  // Profile build configuration must be loaded instead of Debug
#define AK_OPTIMIZED
#pragma comment( lib, "AK/Release(StaticCRT)/lib/AkSoundEngine.lib")
#pragma comment( lib, "AK/Release(StaticCRT)/lib/AkMusicEngine.lib")
#pragma comment( lib, "AK/Release(StaticCRT)/lib/AkMemoryMgr.lib")
#pragma comment( lib, "AK/Release(StaticCRT)/lib/AkStreamMgr.lib")

#else
#include <AK/Comm/AkCommunication.h>
#pragma comment( lib, "AK/Debug(StaticCRT)/lib/CommunicationCentral.lib" )
#pragma comment( lib, "AK/Debug(StaticCRT)/lib/AkSoundEngine.lib" )
#pragma comment( lib, "AK/Debug(StaticCRT)/lib/AkMusicEngine.lib" )
#pragma comment( lib, "AK/Debug(StaticCRT)/lib/AkMemoryMgr.lib" )
#pragma comment( lib, "AK/Debug(StaticCRT)/lib/AkStreamMgr.lib" )

#endif

#pragma comment( lib, "AK/DirectSound/dinput8.lib" )
#pragma comment( lib, "AK/DirectSound/dsound.lib" ) 
#pragma comment( lib, "AK/DirectSound/dxguid.lib" )

// Wwise stuff
namespace AK
{
#ifdef WIN32
	void * AllocHook(size_t in_size);
	void FreeHook(void * in_ptr);

	void * VirtualAllocHook(void * in_pMemAddress, size_t in_size, DWORD in_dwAllocationType, DWORD in_dwProtect);
	void VirtualFreeHook(void * in_pMemAddress, size_t in_size, DWORD in_dwFreeType);
#endif
}

// Make wwise methods here :)
// We will access all the functions in the engine from here

#endif