#ifndef COMAUDIOSOURCE_H
#define COMAUDIOSOURCE_H

#include "Component.h"
#include "Globals.h"
#include "WwiseT.h"
#include "Timer.h"

class GameObject;

class CompAudioSource : public Component
{
public:
	CompAudioSource(GameObject* parent, COMP_TYPE type, const char* name);
	CompAudioSource(GameObject* parent, COMP_TYPE type);
	~CompAudioSource();
	void Update(float dt);

	void UpdateSourcePos();

	//Getters
	std::string GetAudioToPlay() const;
	bool isMuted()const;
	bool GetBypassEffects()const;
	bool GetPlayOnAwake()const;
	bool isInLoop()const;
	int GetPriority()const;
	float GetVolume()const;
	bool isMono()const;
	float GetPitch()const;
	int GetStereoPanLeft()const;
	int GetStereoPanRight()const;
	float GetMinDistance()const;
	float GetMaxDistance()const;
	float GetMusicSwapTime()const;

	//Seters
	void SetAudio(const char* audio);
	void SetMuted(bool must_mute);
	void SetBypassEffects(bool must_bypass_effects);
	void SetPlayOnAwake(bool must_play_on_awake);
	void SetLoop(bool must_loop);
	void SetPriority(int desired_priority);
	void SetVolume(float desired_volume);
	void SetMono(bool must_mono);
	void SetPitch(float desired_pitch);
	void SetStereoPanLeft(float desired_stereo_pan);
	void SetStereoPanRight(float desired_stereo_pan);
	void SetMinDistance(float desired_min_distance);
	void SetMaxDistance(float desired_max_distance);
	void SetMusicSwapTimer(float new_time);

	void PlayAudio();
	void PauseAudio();
	void ResumeAudio();
	void StopAudio();

	void DebugDraw();

	//Serialization
	uint GetSize() const;
	void WriteComponentData(char ** cursor);

public:
	WwiseT::AudioSource* source;

private:
	//const char* audio_to_play = "music_TheGrowlers_GoingGetsTuff";
	bool mute = false;
	bool bypass_effects = true;
	bool play_on_awake = true;
	bool loop = false;
	int priority = 50;
	float volume = 1;
	bool mono = false;
	float pitch = 0.0f;
	float stereo_pan_l = 0.0f;
	float stereo_pan_r = 0.0f;
	float min_distance = 1.0f;
	float max_distance = 500.0f;
	std::string audio_to_play;
	Timer timer;
	float time_to_swap = 30.0f;
	int current_state = 2;
};

#endif // !COMAUDIOSOURCE_H