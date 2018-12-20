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

	//Getters
	const char* GetAudioToPlay() const;
	bool isMuted()const;
	bool GetBypassEffects()const;
	bool GetPlayOnAwake()const;
	bool isInLoop()const;
	int GetPriority()const;
	float GetVolume()const;
	bool isMono()const;
	int GetPitch()const;
	int GetStereoPan()const;
	float GetMinDistance()const;
	float GetMaxDistance()const;

	//Seters
	void SetAudio(const char* audio);
	void SetMuted(bool must_mute);
	void SetBypassEffects(bool must_bypass_effects);
	void SetPlayOnAwake(bool must_play_on_awake);
	void SetLoop(bool must_loop);
	void SetPriority(int desired_priority);
	void SetVolume(float desired_volume);
	void SetMono(bool must_mono);
	void SetPitch(int desired_pitch);
	void SetStereoPan(int desired_stereo_pan);
	void SetMinDistance(float desired_min_distance);
	void SetMaxDistance(float desired_max_distance);

	void PlayAudio();
	void PauseAudio();
	void ResumeAudio();
	void StopAudio();

public:
	WwiseT::AudioSource* source;
	uint audio_source_id;

private:
	const char* audio_to_play = "music_TheGrowlers_GoingGetsTuff";
	bool mute = false;
	bool bypass_effects = true;
	bool play_on_awake = true;
	bool loop = false;
	int priority = 50;
	float volume = 1;
	bool mono = false;
	int pitch = 0;
	int stereo_pan = 0;
	float min_distance = 1.0f;
	float max_distance = 500.0f;

};

#endif // !COMAUDIOSOURCE_H