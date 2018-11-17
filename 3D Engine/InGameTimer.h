#ifndef __INGAMETIMER_H__
#define __INGAMETIMER_H__

#include "Globals.h"
#include "SDL/include/SDL.h"

class InGameTimer
{
public:

	// Constructor
	InGameTimer();

	void UpdateTimer();

	void Start();
	void Pause();
	void UnPause();
	void Stop();
	void SetTimerScale(float new_scale);
	float GetTimerScale() const;
	float GetDT() const;

	bool IsPaused()const;

	void ResetExtraTicks();
	void AddExtraTicks(float ticks);

	Uint32 Read();

private:

	bool	running;
	bool	paused = false;
	Uint32	started_at;
	Uint32	paused_at;
	Uint32	stopped_at;
	Uint32	time;
	float   time_scale = 1.0f;
	float dt = 0.0f;
	float extra_ticks = 0.0f;
};

#endif //__INGAMETIMER_H__