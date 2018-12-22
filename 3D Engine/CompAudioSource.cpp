#include "CompAudioSource.h"
#include "Application.h"
#include "ModuleAudio.h"
#include "CompTransform.h"
#include "MathGeoLib\MathGeoLib.h"

CompAudioSource::CompAudioSource(GameObject * parent, COMP_TYPE type, const char* name) : Component(parent, type)
{
	source = WwiseT::CreateAudSource(name);
}

CompAudioSource::CompAudioSource(GameObject * parent, COMP_TYPE type) : Component(parent, type)
{
}

CompAudioSource::~CompAudioSource()
{
	audio_to_play.clear();
}

void CompAudioSource::Update(float dt)
{
	UpdateSourcePos();

	DebugDraw();
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
int CompAudioSource::GetStereoPanLeft()const
{
	return stereo_pan_l;
}
int CompAudioSource::GetStereoPanRight()const
{
	return stereo_pan_r;
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
void CompAudioSource::SetStereoPanLeft(float desired_stereo_pan)
{
	stereo_pan_l = desired_stereo_pan;
	source->SetPanLeft(desired_stereo_pan);
}

void CompAudioSource::SetStereoPanRight(float desired_stereo_pan)
{
	stereo_pan_r = desired_stereo_pan;
	source->SetPanRight(desired_stereo_pan);
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

uint CompAudioSource::GetSize() const
{
	// GET SIZE FOR SERIALIZATION
	uint size = 0;

	// COMPONENT TYPE
	size += sizeof(int);
	// IS ACTIVE
	size += sizeof(int);
	// AUDIO TO PLAY
	size += sizeof(char)*128;
	// SOURCE ID
	size += sizeof(uint);
	// MUTE
	size += sizeof(int);
	// PLAY ON AWAKE
	size += sizeof(int);
	// PRIORITY
	size += sizeof(int);
	// VOLUME
	size += sizeof(float);
	// PITCH
	size += sizeof(float);
	// STEREO PAN L
	size += sizeof(float);
	// STEREO PAN R
	size += sizeof(float);
	// MIN DISTANCE
	size += sizeof(float);
	// MAX DISTANCE
	size += sizeof(float);

	return size;;
}

void CompAudioSource::WriteComponentData(char ** cursor)
{
	uint bytes = 0;

	// COMPONENT TYPE
	bytes = sizeof(int);
	memcpy(cursor[0], &type, bytes);
	cursor[0] += bytes;
	// IS ACTIVE
	bytes = sizeof(int);
	int tmp_active = (int)active;
	memcpy(cursor[0], &tmp_active, bytes);
	cursor[0] += bytes;
	// AUDIO TO PLAY
	bytes = sizeof(char) * 128;
	char* name = new char[128];
	strcpy(name, audio_to_play.c_str());
	memcpy(cursor[0], name, bytes);
	cursor[0] += bytes;
	// SOURCE ID
	bytes = sizeof(uint);
	uint tmp_id = (uint)source->GetID();
	memcpy(cursor[0], &tmp_id, bytes);
	cursor[0] += bytes;
	// MUTE
	bytes = sizeof(int);
	int tmp_mute = (int)mute;
	memcpy(cursor[0], &tmp_mute, bytes);
	cursor[0] += bytes;
	// PLAY ON AWAKE
	bytes = sizeof(int);
	int tmp_poa = (int)play_on_awake;
	memcpy(cursor[0], &tmp_poa, bytes);
	cursor[0] += bytes;
	// PRIORITY
	bytes = sizeof(int);
	int tmp_priority = (int)priority;
	memcpy(cursor[0], &tmp_priority, bytes);
	cursor[0] += bytes;
	// VOLUME
	bytes = sizeof(float);
	float tmp_volume = (float)volume;
	memcpy(cursor[0], &tmp_volume, bytes);
	cursor[0] += bytes;
	// PITCH
	bytes = sizeof(float);
	float tmp_pitch = (float)pitch;
	memcpy(cursor[0], &tmp_pitch, bytes);
	cursor[0] += bytes;
	// STEREO PAN L
	bytes = sizeof(float);
	float tmp_panl = (float)stereo_pan_l;
	memcpy(cursor[0], &tmp_panl, bytes);
	cursor[0] += bytes;
	// STEREO PAN R
	bytes = sizeof(float);
	float tmp_panr = (float)stereo_pan_r;
	memcpy(cursor[0], &tmp_panr, bytes);
	cursor[0] += bytes;
	// MIN DISTANCE
	bytes = sizeof(float);
	float tmp_min_distance = (float)min_distance;
	memcpy(cursor[0], &tmp_min_distance, bytes);
	cursor[0] += bytes;
	// MAX DISTANCE
	bytes = sizeof(float);
	float tmp_max_distance = (float)max_distance;
	memcpy(cursor[0], &tmp_max_distance, bytes);
	cursor[0] += bytes;
	
}

void CompAudioSource::DebugDraw() {
	math::Sphere sphere;

	sphere.pos = parent->GetCompTransform()->GetPosition();
	sphere.r = 0.15;

	glLineWidth(3.0f);
	glColor3f(0.0f, 0.0f, 2.0f);

	float radius = sphere.r;
	float3 pos = sphere.pos;
	float degInRad = 360.0f / 12;
	degInRad *= DEGTORAD;
	glBegin(GL_LINE_LOOP);
	for (unsigned int i = 0; i < 12; i++)
		glVertex3f(cos(degInRad * i) * radius + pos.x, pos.y, sin(degInRad * i) * radius + pos.z);
	glEnd();
	glBegin(GL_LINE_LOOP);
	for (unsigned int i = 0; i < 12; i++)
		glVertex3f(cos(degInRad * i) * radius + pos.x, sin(degInRad * i) * radius + pos.y, pos.z);
	glEnd();
	glBegin(GL_LINE_LOOP);
	for (unsigned int i = 0; i < 12; i++)
		glVertex3f(pos.x, sin(degInRad * i) * radius + pos.y, cos(degInRad * i) * radius + pos.z);
	glEnd();

	glLineWidth(1.0f);
}