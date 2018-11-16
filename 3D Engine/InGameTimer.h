#ifndef __INGAMETIMER_H__
#define __INGAMETIMER_H__

#include "Globals.h"
#include "SDL/include/SDL.h"

class InGameTimer
{
public:

	// Constructor
	InGameTimer();

	void Start();
	void Pause();
	void Stop();
	void SetTimerScale(float new_scale);
	float GetTimerScale() const;

	Uint32 Read();

private:

	bool	running;
	Uint32	started_at;
	Uint32	stopped_at;
	float   time_scale = 1.0f;
};

#endif //__INGAMETIMER_H__