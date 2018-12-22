#ifndef COMAUDIOLISTENER_H
#define COMAUDIOLISTENER_H

#include "Component.h"
#include "Globals.h"
#include "WwiseT.h"

class GameObject;

class CompAudioListener : public Component
{
public:
	CompAudioListener(GameObject* parent, COMP_TYPE type);
	~CompAudioListener();
	void Update(float dt);

	void UpdateListenerPos();

	//Serialization
	uint GetSize() const;
	void WriteComponentData(char ** cursor);

	void DebugDraw();

public:
	WwiseT::AudioSource* listener;
};

#endif // !COMCAMERA_H