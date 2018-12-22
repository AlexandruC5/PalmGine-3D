#include "Globals.h"
#include "Application.h"
#include "ModuleAudio.h"

ModuleAudio::ModuleAudio(bool start_enabled) : Module(start_enabled)
{
	
}

ModuleAudio::~ModuleAudio()
{}

bool ModuleAudio::Start()
{
	// Init wwise and audio banks
	WwiseT::InitSoundEngine();
	WwiseT::LoadBank("Assignment3.bnk");
	return true;
}

update_status ModuleAudio::Update(float dt)
{
	return UPDATE_CONTINUE;
}

update_status ModuleAudio::PostUpdate(float dt)
{
	WwiseT::ProcessAudio();
	return UPDATE_CONTINUE;
}

WwiseT::AudioSource * ModuleAudio::CreateSoundEmitter(const char * name)
{
	WwiseT::AudioSource* ret = WwiseT::CreateAudSource(name);
	event_list.push_back(ret);
	return ret;
}

uint ModuleAudio::GetListenerID() const
{
	return listener->GetID();
}

void ModuleAudio::Stop() const
{

}

void ModuleAudio::Pause() const
{
}

void ModuleAudio::Resume() const
{
}
