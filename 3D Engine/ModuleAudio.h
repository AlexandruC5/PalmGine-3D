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

	// Set listener
	// Create emmiter
	// Delete emmiter
	
	WwiseT::AudioSource* test2 = nullptr;
private:
	std::list<WwiseT::AudioSource*> event_list;
	// Keep listener?
};

#endif // !MODULEAUDIO_H