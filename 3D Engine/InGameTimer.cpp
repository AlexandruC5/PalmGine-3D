// ----------------------------------------------------
// Timer.cpp
// Body for CPU Tick Timer class
// ----------------------------------------------------

#include "InGameTimer.h"

// ---------------------------------------------
InGameTimer::InGameTimer()
{
	Start();
}

// ---------------------------------------------
void InGameTimer::Start()
{
	running = true;
	started_at = SDL_GetTicks();
}

// ---------------------------------------------
void InGameTimer::Pause() {
	running = !running;
}

// ---------------------------------------------
void InGameTimer::Stop()
{
	running = false;
	stopped_at = SDL_GetTicks();
}

// ---------------------------------------------
void InGameTimer::SetTimerScale(float new_scale) {
	time_scale = new_scale;
}

float InGameTimer::GetTimerScale() const {
	return time_scale;
}

// ---------------------------------------------
Uint32 InGameTimer::Read()
{
	if(running == true)
	{
		return SDL_GetTicks() - started_at;
	}
	else
	{
		return stopped_at - started_at;
	}
}


