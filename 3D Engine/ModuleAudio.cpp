#include "Globals.h"
#include "Application.h"
#include "ModuleAudio.h"

ModuleAudio::ModuleAudio(bool start_enabled) : Module(start_enabled)
{}

ModuleAudio::~ModuleAudio()
{
	// clean up all emmiters
}

bool ModuleAudio::Start()
{
	return true;
}

update_status ModuleAudio::Update(float dt)
{
	return UPDATE_CONTINUE;
}