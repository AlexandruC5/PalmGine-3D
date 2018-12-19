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
	test2->SetListener();
	return true;
}

update_status ModuleAudio::Update(float dt)
{
	//TODO delete test sound
	if ((App->input->GetKey(SDL_SCANCODE_P) == KEY_DOWN))
	{
		test2->PlayEventByName("fireball");
	}
	
	WwiseT::ProcessAudio();
	return UPDATE_CONTINUE;
}