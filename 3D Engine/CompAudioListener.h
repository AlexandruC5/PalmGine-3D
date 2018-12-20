#ifndef COMAUDIOLISTENER_H
#define COMAUDIOLISTENER_H

#include "Component.h"
#include "Globals.h"
#include <vector>
#include "MathGeoLib/MathGeoLib.h"

class GameObject;

class CompAudioListener : public Component
{
public:
	CompAudioListener(GameObject* parent, COMP_TYPE type);
	~CompAudioListener();

public:
	WwiseT::AudioSource* listener;
	uint audio_listener_id;
};

#endif // !COMCAMERA_H