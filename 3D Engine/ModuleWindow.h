#ifndef __ModuleWindow_H__
#define __ModuleWindow_H__

#include "Module.h"
#include "SDL/include/SDL.h"

class Application;

class ModuleWindow : public Module
{
public:

	ModuleWindow( bool start_enabled = true);

	// Destructor
	virtual ~ModuleWindow();

	bool Init();
	bool CleanUp();

	void SetTitle(const char* title);

	void SetFullScreen(bool fullscreen);
	void SetWindowed(bool borderless);
	void SetWindowFullDesktop();

public:
	//The window we'll be rendering to
	SDL_Window* window;

	//The surface contained by the window
	SDL_Surface* screen_surface;

	bool fullscreen = false;
	bool windowed = false;
	bool full_desktop = false;

	int width = 0;
	int height = 0;
	int scale = 0;

};

#endif // __ModuleWindow_H__