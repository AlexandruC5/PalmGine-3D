#include "WwiseT.h"

#include <AK/SoundEngine/Common/AkMemoryMgr.h>                  // Memory Manager
#include <AK/SoundEngine/Common/AkModule.h>						// Default memory and stream managers
#include "AK/Win32/AkFilePackageLowLevelIOBlocking.h"			
#include <AK/MusicEngine/Common/AkMusicEngine.h>                // Music Engine
#include <assert.h>

CAkFilePackageLowLevelIOBlocking g_lowLevelIO;

// Wwise memory hooks
namespace AK
{
	void * AllocHook(size_t in_size)
	{
		return malloc(in_size);
	}

	void FreeHook(void * in_ptr)
	{
		free(in_ptr);
	}

	void * VirtualAllocHook(
		void * in_pMemAddress,
		size_t in_size,
		DWORD in_dwAllocationType,
		DWORD in_dwProtect
	)
	{
		return VirtualAlloc(in_pMemAddress, in_size, in_dwAllocationType, in_dwProtect);
	}
	void VirtualFreeHook(
		void * in_pMemAddress,
		size_t in_size,
		DWORD in_dwFreeType
	)
	{
		VirtualFree(in_pMemAddress, in_size, in_dwFreeType);
	}
}

// Wwise stuff
bool WwiseT::InitSoundEngine()
{
	// Init mem manager
	AkMemSettings memSettings;
	memSettings.uMaxNumPools = 20;

	if (AK::MemoryMgr::Init(&memSettings) != AK_Success)
	{
		assert(!"Could not create the memory manager.");
		return false;
	}

	// Init streaming manager
	AkStreamMgrSettings stmSettings;
	AK::StreamMgr::GetDefaultSettings(stmSettings);

	// Customize the Stream Manager settings here.

	if (!AK::StreamMgr::Create(stmSettings))
	{
		assert(!"Could not create the Streaming Manager");
		return false;
	}
	AkDeviceSettings deviceSettings;
	AK::StreamMgr::GetDefaultDeviceSettings(deviceSettings);

	if (g_lowLevelIO.Init(deviceSettings) != AK_Success)
	{
		assert(!"Could not create the streaming device and Low-Level I/O system");
		return false;
	}

	// Init sound engine
	AkInitSettings initSettings;
	AkPlatformInitSettings platformInitSettings;
	AK::SoundEngine::GetDefaultInitSettings(initSettings);
	AK::SoundEngine::GetDefaultPlatformInitSettings(platformInitSettings);

	if (AK::SoundEngine::Init(&initSettings, &platformInitSettings) != AK_Success)
	{
		assert(!"Could not initialize the Sound Engine.");
		return false;
	}

	//Init music engine
	AkMusicSettings musicInit;
	AK::MusicEngine::GetDefaultInitSettings(musicInit);

	if (AK::MusicEngine::Init(&musicInit) != AK_Success)
	{
		assert(!"Could not initialize the Music Engine.");
		return false;
	}

	// Initialize communications (not in release build!)
#ifndef AK_OPTIMIZED
	AkCommSettings commSettings;
	AK::Comm::GetDefaultInitSettings(commSettings);
	if (AK::Comm::Init(commSettings) != AK_Success)
	{
		assert(!"Could not initialize communication.");
		return false;
	}
#endif // AK_OPTIMIZED

	AKRESULT base_path_res = g_lowLevelIO.SetBasePath(AKTEXT("../Game/SoundBanks/"));
	if (base_path_res != AK_Success)
	{
		assert(!"Invalid base path!");
		return false;
	}

	// Load the init soundbank
	LoadBank("Init.bnk");

	return true;
}

bool WwiseT::CloseSoundEngine()
{
	//Terminate comunication module (IMPORTANT: this must be the first module in being terminated)
#ifndef AK_OPTIMIZED
	AK::Comm::Term();
#endif 

	// Terminate the music engine
	AK::MusicEngine::Term();

	// Terminate the sound engine
	AK::SoundEngine::Term();

	// Terminate IO device
	g_lowLevelIO.Term();
	if (AK::IAkStreamMgr::Get())
		AK::IAkStreamMgr::Get()->Destroy();

	// Terminate the Memory Manager
	AK::MemoryMgr::Term();

	return true;
}

void WwiseT::ProcessAudio()
{
	AK::SoundEngine::RenderAudio();
}

void WwiseT::LoadBank(const char * path)
{
	AkBankID bankID; // Not used. These banks can be unloaded with their file name.
	AKRESULT eResult = AK::SoundEngine::LoadBank(path, AK_DEFAULT_POOL_ID, bankID);
	if (eResult != AK_Success)
	{
		assert(!"Could not initialize soundbank.");
	}
}

void WwiseT::SetDefaultListener(uint id)
{
	AkGameObjectID tmp = id;
	AKRESULT eResult = AK::SoundEngine::SetDefaultListeners(&tmp, 1);
	if (eResult != AK_Success)
	{
		assert(!"Could not set GameObject as default listerner.");
	}
}

WwiseT::AudioSource* WwiseT::CreateAudSource(const char * name)
{
	AudioSource* src = nullptr;

	src = new AudioSource(name);

	return src;
}

WwiseT::AudioSource::AudioSource(const char* event_name)
{
	id = GenRandomNumber();
	name = event_name;
	AKRESULT eResult = AK::SoundEngine::RegisterGameObj(id, name);
	if (eResult != AK_Success)
	{
		assert(!"Could not register GameObject. See eResult variable to more info");
	}
}

WwiseT::AudioSource::~AudioSource()
{
	AKRESULT eResult = AK::SoundEngine::UnregisterGameObj(id);
	if (eResult != AK_Success)
	{
		assert(!"Could not unregister GameObject. See eResult variable to more info");
	}
}

void WwiseT::AudioSource::PlayEventByName(const char * name)
{
	AK::SoundEngine::PostEvent(name, id);
}

void WwiseT::AudioSource::SetListener()
{
	AkGameObjectID tmp = id;
	AKRESULT eResult = AK::SoundEngine::SetListeners(id, &tmp, 1);
	if (eResult != AK_Success)
	{
		assert(!"Could not set GameObject as listerner.");
	}
}

uint WwiseT::AudioSource::GetID()
{
	return id;
}
