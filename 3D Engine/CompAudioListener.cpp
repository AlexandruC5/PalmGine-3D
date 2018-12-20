#include "CompAudioListener.h"
#include "Application.h"
#include "ModuleAudio.h"
#include "CompTransform.h"
#include "MathGeoLib\MathGeoLib.h"

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

void CompAudioListener::Update(float dt)
{
	UpdateListenerPos();
}

void CompAudioListener::UpdateListenerPos()
{
	AkListenerPosition listener_pos;

	CompTransform* transformation = parent->GetCompTransform();

	if (transformation != nullptr) 
	{
		math::Quat rot = transformation->GetRotationQuat();

		math::float3 vector_pos = transformation->GetPosition();
		math::float3 vector_front = rot.Transform(math::float3(0, 0, 1));
		math::float3 vector_up = rot.Transform(math::float3(0, 1, 0));

		listener_pos.Set(vector_pos.x, vector_pos.z, vector_pos.y, vector_front.x, vector_front.z, vector_front.y, vector_up.x, vector_up.z, vector_up.y);
	}

}