#ifndef COMAUDIOSOURCE_H
#define COMAUDIOSOURCE_H

#include "Component.h"
#include "Globals.h"
#include "WwiseT.h"

class GameObject;

class CompAudioSource : public Component
{
public:
	CompAudioSource(GameObject* parent, COMP_TYPE type, const char* name);
	~CompAudioSource();
	void Update(float dt);

	void UpdateSourcePos();

	void SetAudio(const char* audio);

public:
	WwiseT::AudioSource* source;
	uint audio_source_id;
	const char* audio_to_play;
};

#endif // !COMAUDIOSOURCE_H