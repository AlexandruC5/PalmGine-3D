#ifndef MODULEAUDIO_H
#define MODULEAUDIO_H

#include "Module.h"
#include "Globals.h"
#include "WwiseT.h"

class ModuleAudio : public Module
{
public:
	ModuleAudio(bool start_enabled = true);
	~ModuleAudio();
	bool Start();
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

	// Set listener
	WwiseT::AudioSource* CreateSoundEmitter(const char * name);
	uint GetListenerID()const;
	
	// Utils
	void PlayOnAwake()const;
	void Stop()const;
	void Pause()const;
	void Resume()const;
	void SetListener(uint id)const;
	
private:
	std::list<WwiseT::AudioSource*> event_list;
	WwiseT::AudioSource* listener;

public:
	std::list<CompAudioSource*> audio_sources;
};

#endif // !MODULEAUDIO_H