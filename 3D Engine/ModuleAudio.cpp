#include "Globals.h"
#include "Application.h"
#include "ModuleAudio.h"

ModuleAudio::ModuleAudio(bool start_enabled) : Module(start_enabled)
{}

ModuleAudio::~ModuleAudio()
{
	// clean up all emmiters
	delete test2;
	WwiseT::CloseSoundEngine();
}

bool ModuleAudio::Start()
{
	// Init wwise and audio banks
	WwiseT::InitSoundEngine();
	WwiseT::LoadBank("Assignment3.bnk");
	
	//TODO delete test sound
	test2 = WwiseT::CreateAudSource("test");
	WwiseT::SetDefaultListener(test2->GetID());
	//test2->SetListener();
	return true;
}

update_status ModuleAudio::Update(float dt)
{
	//TODO delete test sound
	if ((App->input->GetKey(SDL_SCANCODE_P) == KEY_DOWN))
	{
		test2->PlayEventByName("train");
		float vel = 12;
		test2->ApplyEnvReverb(vel, "tunnel");
	}
	return UPDATE_CONTINUE;
}

update_status ModuleAudio::PostUpdate(float dt)
{
	WwiseT::ProcessAudio();
	return UPDATE_CONTINUE;
}
