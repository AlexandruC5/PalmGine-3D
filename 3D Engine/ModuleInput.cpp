#include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"
#include "imGUI/imgui_impl_sdl_gl3.h"

#define MAX_KEYS 300

ModuleInput::ModuleInput(bool start_enabled) : Module(start_enabled)
{
	keyboard = new KEY_STATE[MAX_KEYS];
	memset(keyboard, KEY_IDLE, sizeof(KEY_STATE) * MAX_KEYS);
	memset(mouse_buttons, KEY_IDLE, sizeof(KEY_STATE) * MAX_MOUSE_BUTTONS);
}

// Destructor
ModuleInput::~ModuleInput()
{
	delete[] keyboard;
}

// Called before render is available
bool ModuleInput::Init()
{
	LOG("Init SDL input event system");
	bool ret = true;
	SDL_Init(0);

	if(SDL_InitSubSystem(SDL_INIT_EVENTS) < 0)
	{
		LOG("SDL_EVENTS could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	return ret;
}

// Called every draw update
update_status ModuleInput::PreUpdate(float dt)
{
	SDL_PumpEvents();

	const Uint8* keys = SDL_GetKeyboardState(NULL);
	
	for(int i = 0; i < MAX_KEYS; ++i)
	{
		if(keys[i] == 1)
		{
			if(keyboard[i] == KEY_IDLE)
				keyboard[i] = KEY_DOWN;
			else
				keyboard[i] = KEY_REPEAT;
		}
		else
		{
			if(keyboard[i] == KEY_REPEAT || keyboard[i] == KEY_DOWN)
				keyboard[i] = KEY_UP;
			else
				keyboard[i] = KEY_IDLE;
		}
	}

	Uint32 buttons = SDL_GetMouseState(&mouse_x, &mouse_y);

	mouse_x /= App->window->scale;
	mouse_y /= App->window->scale;
	mouse_z = 0;

	for(int i = 0; i < 5; ++i)
	{
		if(buttons & SDL_BUTTON(i))
		{
			if(mouse_buttons[i] == KEY_IDLE)
				mouse_buttons[i] = KEY_DOWN;
			else
				mouse_buttons[i] = KEY_REPEAT;
		}
		else
		{
			if(mouse_buttons[i] == KEY_REPEAT || mouse_buttons[i] == KEY_DOWN)
				mouse_buttons[i] = KEY_UP;
			else
				mouse_buttons[i] = KEY_IDLE;
		}
	}

	mouse_x_motion = mouse_y_motion = 0;

	bool quit = false;
	SDL_Event e;
	while(SDL_PollEvent(&e))
	{

		ImGui_ImplSdlGL3_ProcessEvent(&e);

		switch(e.type)
		{
			FILE_TYPE file_type;
		case SDL_MOUSEWHEEL:
			mouse_z = e.wheel.y;
			break;

		case SDL_MOUSEMOTION:
			mouse_x = e.motion.x / App->window->scale;
			mouse_y = e.motion.y / App->window->scale;

			mouse_x_motion = e.motion.xrel / App->window->scale;
			mouse_y_motion = e.motion.yrel / App->window->scale;
			break;

		case SDL_QUIT:
			quit = true;
			break;

		case SDL_DROPFILE:
			fbx_path = e.drop.file;
			file_type = GetFileType(fbx_path.c_str());
			if (file_type == GEOMETRY_MODEL)
			{
				App->importer->LoadFBX(fbx_path.c_str());
			}
			else if (file_type == TEXTURE)
			{
				// Set windows path to import images
				std::string path = "..//Textures//";
				path += GetFileNameWithExtension(fbx_path.c_str());
				App->importer->ImportImage(path.c_str());
			}

		case SDL_WINDOWEVENT:
		{
			if (e.window.event == SDL_WINDOWEVENT_RESIZED)
				App->renderer3D->OnResize(e.window.data1, e.window.data2);
		}
		}
	}

	if(quit == true || keyboard[SDL_SCANCODE_ESCAPE] == KEY_UP)
		return UPDATE_STOP;

	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleInput::CleanUp()
{
	LOG("Quitting SDL input event subsystem");
	SDL_QuitSubSystem(SDL_INIT_EVENTS);
	return true;
}

const FILE_TYPE ModuleInput::GetFileType(const char * dir) const
{
	if (dir != nullptr)
	{
		std::string type;
		//From const char* to std::string
		std::string path(dir);
		//Find extension
		type = path.substr(path.find_last_of("."));
		if (type == ".png" || type == ".jpg" || type == ".bmp" || type == ".dds" || type == ".tga" ||
			type == ".PNG" || type == ".JPG" || type == ".BMP" || type == ".DDS" ||  type == ".TGA")
		{
			return TEXTURE;
		}
		else if (type == ".fbx" || type == ".obj" ||
			type == ".FBX" || type == ".OBJ")
		{
			return GEOMETRY_MODEL;
		}
		else
		{
			return UNKNOWN;
		}
	}

	LOG("File directory ERROR. Is nullptr.");
	return DIR_NULLPTR;
}