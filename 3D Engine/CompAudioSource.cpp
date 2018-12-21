#include "CompAudioSource.h"
#include "Application.h"
#include "ModuleAudio.h"
#include "CompTransform.h"
#include "MathGeoLib\MathGeoLib.h"

CompAudioSource::CompAudioSource(GameObject * parent, COMP_TYPE type, const char* name) : Component(parent, type)
{
	source = WwiseT::CreateAudSource(name);
	audio_source_id = source->GetID();
}

CompAudioSource::~CompAudioSource()
{
	delete source;
	audio_to_play.clear();
}

void CompAudioSource::Update(float dt)
{
	UpdateSourcePos();

	if (audio_to_play != "" && loop == true) {
		source->PlayEventByName(audio_to_play.c_str());
	}

}

void CompAudioSource::UpdateSourcePos()
{
	
	CompTransform* transformation = parent->GetCompTransform();

	if (transformation != nullptr) 
	{
		math::Quat rot = transformation->GetRotationQuat();

		math::float3 vector_pos = transformation->GetPosition();
		math::float3 vector_front = rot.Transform(math::float3(0, 0, 1));
		math::float3 vector_up = rot.Transform(math::float3(0, 1, 0));

		source->SetPos(vector_pos.x, vector_pos.y, vector_pos.z, vector_front.x, vector_front.y, vector_front.z, vector_up.x, vector_up.y, vector_up.z);
	}

}

//Getters
std::string CompAudioSource::GetAudioToPlay() const
{
	return audio_to_play;
}
bool CompAudioSource::isMuted()const
{
	return mute;
}
bool CompAudioSource::GetBypassEffects()const
{
	return bypass_effects;
}
bool CompAudioSource::GetPlayOnAwake()const
{
	return play_on_awake;
}
bool CompAudioSource::isInLoop()const
{
	return loop;
}
int CompAudioSource::GetPriority()const
{
	return priority;
}
float CompAudioSource::GetVolume()const
{
	return volume;
}
bool CompAudioSource::isMono()const
{
	return mono;
}
float CompAudioSource::GetPitch()const
{
	return pitch;
}
int CompAudioSource::GetStereoPan()const
{
	return stereo_pan;
}
float CompAudioSource::GetMinDistance()const
{
	return min_distance;
}
float CompAudioSource::GetMaxDistance()const
{
	return max_distance;
}

//Setters
void CompAudioSource::SetAudio(const char* audio)
{
	audio_to_play = audio;
}
void CompAudioSource::SetMuted(bool must_mute)
{
	mute = must_mute;
	if (mute == true) {
		source->SetVolume(0);
	}
	else {
		source->SetVolume(volume);
	}
}
void CompAudioSource::SetBypassEffects(bool must_bypass_effects)
{
	bypass_effects = must_bypass_effects;
}
void CompAudioSource::SetPlayOnAwake(bool must_play_on_awake)
{
	play_on_awake = must_play_on_awake;
}
void CompAudioSource::SetLoop(bool must_loop)
{
	loop = must_loop;
}
void CompAudioSource::SetPriority(int desired_priority) 
{
	priority = desired_priority;
}
void CompAudioSource::SetVolume(float desired_volume)
{
	volume = desired_volume;
	if (mute == false) {
		source->SetVolume(desired_volume);
	}
}
void CompAudioSource::SetMono(bool must_mono)
{
	mono = must_mono;
	if (mono == true) {
		source->SetMono();
	}
	else {
		source->SetStereo();
	}
}
void CompAudioSource::SetPitch(float desired_pitch)
{
	pitch = desired_pitch;
	source->SetPitch(pitch);
}
void CompAudioSource::SetStereoPan(int desired_stereo_pan)
{
	stereo_pan = desired_stereo_pan;
}
void CompAudioSource::SetMinDistance(float desired_min_distance)
{
	min_distance = desired_min_distance;
}
void CompAudioSource::SetMaxDistance(float desired_max_distance)
{
	max_distance = desired_max_distance;
}

void CompAudioSource::PlayAudio()
{
	source->PlayEventByName(audio_to_play.c_str());
}

void CompAudioSource::PauseAudio()
{
	source->PauseEventByName(audio_to_play.c_str());
}

void CompAudioSource::ResumeAudio()
{
	source->ResumeEventByName(audio_to_play.c_str());
}

void CompAudioSource::StopAudio()
{
	source->StopEventByName(audio_to_play.c_str());
}