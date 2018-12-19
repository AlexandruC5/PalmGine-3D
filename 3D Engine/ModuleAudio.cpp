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
	// Init wwise and audio banks
	WwiseT::InitSoundEngine();
	WwiseT::LoadBank("Assignment3.bnk");
	
	//TODO delete test sound
	WwiseT::AudioSource* test = WwiseT::CreateAudSource("test");
	event_list.push_back(test);

	return true;
}

update_status ModuleAudio::Update(float dt)
{
	return UPDATE_CONTINUE;
}