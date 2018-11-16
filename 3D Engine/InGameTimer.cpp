// ----------------------------------------------------
// Timer.cpp
// Body for CPU Tick Timer class
// ----------------------------------------------------

#include "InGameTimer.h"
#include "Application.h"

// ---------------------------------------------
InGameTimer::InGameTimer()
{
	Start();
}

void InGameTimer::UpdateTimer() {
	if (paused == true || running == false) {
		dt = 0.0f;
	}
	else {
		dt = App->GetDT() * time_scale;
	}
}

// ---------------------------------------------
void InGameTimer::Start()
{
	paused = false;
	running = true;
	started_at = SDL_GetTicks();
}

// ---------------------------------------------
void InGameTimer::Pause() {
	paused = true;
	paused_at = SDL_GetTicks();
}

// ---------------------------------------------
void InGameTimer::UnPause() {
	paused = false;
}

// ---------------------------------------------
void InGameTimer::Stop()
{
	paused = false;
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
	if (paused == true) {
		return paused_at - started_at;
	}
	else if(running == true)
	{
		return SDL_GetTicks() - started_at;
	}
	else
	{
		return stopped_at - started_at;
	}
}

// ---------------------------------------------
bool InGameTimer::IsPaused() const{
	return paused;
}

// ---------------------------------------------
float InGameTimer::GetDT()const {
	return dt;
}