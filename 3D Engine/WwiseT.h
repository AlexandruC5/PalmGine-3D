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
#pragma comment( lib, "AK/ws2_32.lib") 
#pragma comment( lib, "AK/Debug(StaticCRT)/lib/AkSoundEngine.lib" )
#pragma comment( lib, "AK/Debug(StaticCRT)/lib/AkMusicEngine.lib" )
#pragma comment( lib, "AK/Debug(StaticCRT)/lib/AkMemoryMgr.lib" )
#pragma comment( lib, "AK/Debug(StaticCRT)/lib/AkStreamMgr.lib" )

#endif

#pragma comment( lib, "AK/DirectSound/dinput8.lib" )
#pragma comment( lib, "AK/DirectSound/dsound.lib" ) 
#pragma comment( lib, "AK/DirectSound/dxguid.lib" )

// Wwise memory hooks
// From wwise tutorial: https://www.audiokinetic.com/library/edge/?source=SDK&id=workingwithsdks__initialization.html
namespace AK
{
#ifdef WIN32
	void * AllocHook(size_t in_size);
	void FreeHook(void * in_ptr);

	void * VirtualAllocHook(void * in_pMemAddress, size_t in_size, DWORD in_dwAllocationType, DWORD in_dwProtect);
	void VirtualFreeHook(void * in_pMemAddress, size_t in_size, DWORD in_dwFreeType);
#endif
}

namespace WwiseT
{
	class AudioSource
	{
	public:
		AudioSource(const char* name);
		~AudioSource();


		void PlayByName(const char* name);
	private:
		AkGameObjectID id;
		const char* name = nullptr;
	};

	bool InitSoundEngine();
	void ProcessAudio();
	void TermSoundEngine(); // Close wwise
	void LoadBank(const char* path);
	void SetLanguage(const char* lang);
	AudioSource* CreateAudSource(const char* name);
}
// Make wwise methods here :)
// We will access all the functions in the engine from here

#endif