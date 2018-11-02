#include "Globals.h"
#include "Application.h"
#include "ModuleWindow.h"

ModuleWindow::ModuleWindow(bool start_enabled) : Module(start_enabled)
{
	window = NULL;
	screen_surface = NULL;
}

// Destructor
ModuleWindow::~ModuleWindow()
{
}

// Called before render is available
bool ModuleWindow::Init()
{
	LOG("Init SDL window & surface");
	bool ret = true;

	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		LOG("SDL_VIDEO could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	else
	{
		JSONwindow_obj = json_object_get_object(App->JSONconfig_obj, "window");

		//Create window
		scale = json_object_get_number(JSONwindow_obj, "scale");
		width = json_object_get_number(JSONwindow_obj, "width") * scale;
		height = json_object_get_number(JSONwindow_obj, "height") * scale;
		Uint32 flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;

		//Use OpenGL 2.1
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

		if (json_object_get_boolean(JSONwindow_obj, "fullscreen") == true)
		{
			fullscreen = true;
			flags |= SDL_WINDOW_FULLSCREEN;
		}

		if (json_object_get_boolean(JSONwindow_obj, "resizable") == true)
		{
			resizable = true;
			flags |= SDL_WINDOW_RESIZABLE;
		}

		if (json_object_get_boolean(JSONwindow_obj, "borderless") == true)
		{
			windowed = true;
			flags |= SDL_WINDOW_BORDERLESS;
		}

		if (json_object_get_boolean(JSONwindow_obj, "fullscreen_window") == true)
		{
			full_desktop = true;
			flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
		}

		window = SDL_CreateWindow(json_object_get_string(JSONwindow_obj, "title"), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, flags);

		if(window == NULL)
		{
			LOG("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			ret = false;
		}
		else
		{
			//Get window surface
			screen_surface = SDL_GetWindowSurface(window);
		}
	}

	return ret;
}

// Called before quitting
bool ModuleWindow::CleanUp()
{
	LOG("Destroying SDL window and quitting all SDL systems");

	json_object_set_number(JSONwindow_obj, "scale", scale);
	json_object_set_number(JSONwindow_obj, "width", width / scale);
	json_object_set_number(JSONwindow_obj, "height", height / scale);

	json_object_set_boolean(JSONwindow_obj, "fullscreen", fullscreen);
	json_object_set_boolean(JSONwindow_obj, "resizable", resizable);
	json_object_set_boolean(JSONwindow_obj, "borderless", windowed);
	json_object_set_boolean(JSONwindow_obj, "fullscreen_window", full_desktop);

	JSONwindow_obj = nullptr;

	//Destroy window
	if(window != NULL)
	{
		SDL_DestroyWindow(window);
	}

	//Quit SDL subsystems
	SDL_Quit();
	return true;
}

void ModuleWindow::SetTitle(const char* title)
{
	SDL_SetWindowTitle(window, title);
}

void ModuleWindow::SetFullScreen(bool fullscreen)
{
	if (fullscreen == true)
	{
		SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
	}
	else
	{
		SDL_SetWindowFullscreen(window, 0);
	}

}

void ModuleWindow::SetWindowed(bool borderless)
{
	if (borderless == true)
	{
		SDL_SetWindowFullscreen(window, 0);
	}
}

void ModuleWindow::SetWindowFullDesktop()
{
	SDL_SetWindowSize(window, 1920, 1080);
}