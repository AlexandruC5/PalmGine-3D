#ifndef MODULEAUDIO_H
#define MODULEAUDIO_H

#include "Module.h"
#include "Globals.h"

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

private:
	
	// list of emmiters
	// listener (?
};

#endif // !MODULEAUDIO_H