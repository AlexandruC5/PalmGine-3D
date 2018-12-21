#include "Globals.h"
#include "Application.h"
#include "ModuleAudio.h"

ModuleAudio::ModuleAudio(bool start_enabled) : Module(start_enabled)
{}

ModuleAudio::~ModuleAudio()
{
	// clean up all emmiters
	delete test1;
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
	test1 = WwiseT::CreateAudSource("test1");
	return true;
}

update_status ModuleAudio::Update(float dt)
{
	//TODO delete test sound
	if ((App->input->GetKey(SDL_SCANCODE_P) == KEY_DOWN))
	{
		test1->PlayEventByName("train");
		test2->PlayEventByName("music_TheGrowlers_GoingGetsTuff_stereo");
		float vel = 12;
		test1->ApplyEnvReverb(vel, "tunnel");
	}
	if ((App->input->GetKey(SDL_SCANCODE_L) == KEY_DOWN))
	{
		test2->ChangeState("MusicState", "Off");
	}
	return UPDATE_CONTINUE;
}

update_status ModuleAudio::PostUpdate(float dt)
{
	WwiseT::ProcessAudio();
	return UPDATE_CONTINUE;
}
