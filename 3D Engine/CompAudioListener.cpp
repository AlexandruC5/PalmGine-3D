#include "CompAudioListener.h"
#include "Application.h"
#include "ModuleAudio.h"

CompAudioListener::CompAudioListener(GameObject * parent, COMP_TYPE type) : Component(parent, type)
{
	listener = WwiseT::CreateAudSource("listener");
	audio_listener_id = listener->GetID();
	WwiseT::SetDefaultListener(audio_listener_id);
}

CompAudioListener::~CompAudioListener()
{
	delete listener;
	WwiseT::SetDefaultListener(App->audio->test2->GetID());
}