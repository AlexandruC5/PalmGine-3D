#include "Globals.h"
#include "Application.h"
#include "ModuleRenderer3D.h"
#include "SDL/include/SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>
#include "CompCamera.h"

#pragma comment (lib, "glu32.lib")    /* link OpenGL Utility lib     */
#pragma comment (lib, "opengl32.lib") /* link Microsoft OpenGL lib   */

ModuleRenderer3D::ModuleRenderer3D(bool start_enabled) : Module(start_enabled)
{}

// Destructor
ModuleRenderer3D::~ModuleRenderer3D()
{}

// Called before render is available
bool ModuleRenderer3D::Init()
{
	LOG("Creating 3D Renderer context");
	bool ret = true;
	
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		LOG("SDL_VIDEO could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	else
	{
		//Create context
		context = SDL_GL_CreateContext(App->window->window);
		if (context == NULL)
		{
			LOG("OpenGL context could not be created! SDL_Error: %s\n", SDL_GetError());
			ret = false;
		}

		if (ret == true)
		{
			//Use Vsync
			JSON_Object* JSONrenderer_obj = json_object_get_object(App->JSONconfig_obj, "renderer3D");
			if (json_object_get_boolean(JSONrenderer_obj, "vsync") && SDL_GL_SetSwapInterval(1) < 0)
				LOG("Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());
			JSONrenderer_obj = nullptr;

			//Initialize Projection Matrix
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();

			//Check for error
			GLenum error = glGetError();
			if (error != GL_NO_ERROR)
			{
				LOG("Error initializing OpenGL! %s\n", gluErrorString(error));
				ret = false;
			}

			//Initialize Modelview Matrix
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();

			//Check for error
			error = glGetError();
			if (error != GL_NO_ERROR)
			{
				LOG("Error initializing OpenGL! %s\n", gluErrorString(error));
				ret = false;
			}

			glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
			glClearDepth(1.0f);

			//Initialize clear color
			glClearColor(0.220, 0.220, 0.220, 1.f);

			//Check for error
			error = glGetError();
			if (error != GL_NO_ERROR)
			{
				LOG("Error initializing OpenGL! %s\n", gluErrorString(error));
				ret = false;
			}
			GLfloat LightModelAmbient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
			glLightModelfv(GL_LIGHT_MODEL_AMBIENT, LightModelAmbient);
			//Lights on scene
			lights[0].ref = GL_LIGHT0;
			lights[0].diffuse.Set(0.5f, 0.5f, 0.5f, 1.0f);
			lights[0].ambient.Set(0.9f, 0.9f, 0.9f, 1.0f);
			lights[0].SetPos(0.0f, 0.5f, 0.0f);
			lights[0].Init();

			GLfloat MaterialAmbient[] = { 1.0f, 1.0f, 1.0f, 1.0f };
			glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, MaterialAmbient);

			GLfloat MaterialDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
			glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MaterialDiffuse);

			glEnable(GL_DEPTH_TEST);
			glEnable(GL_CULL_FACE);
			lights[0].Active(true);
			glEnable(GL_LIGHTING);
			glEnable(GL_COLOR_MATERIAL);
		}
		// Projection matrix for
		OnResize(App->window->width, App->window->height);
	}
	return ret;
}

// PreUpdate: clear buffer
update_status ModuleRenderer3D::PreUpdate(float dt)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);
	if (App->scene_intro->game_running == false) {
		glLoadMatrixf((float*)&App->camera->GetViewMatrix());
	}
	else {
		glLoadMatrixf((float*)&App->scene_intro->camera->GetCompCamera()->GetViewMatrix());
	}

	// light 0 on cam pos
	lights[0].SetPos(App->camera->GetCameraPos().x, App->camera->GetCameraPos().y, App->camera->GetCameraPos().z);

	for(uint i = 0; i < MAX_LIGHTS; ++i)
		lights[i].Render();

	return UPDATE_CONTINUE;
}

// Update
update_status ModuleRenderer3D::Update(float dt)
{
	return UPDATE_CONTINUE;
}

// PostUpdate present buffer to screen
update_status ModuleRenderer3D::PostUpdate(float dt)
{
	SDL_GL_SwapWindow(App->window->window);
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleRenderer3D::CleanUp()
{
	LOG("Destroying 3D Renderer");

	SDL_GL_DeleteContext(context);

	return true;
}

void ModuleRenderer3D::OnResize(int width, int height)
{
	glViewport(0, 0, width, height);
	App->window->width = width;
	App->window->height = height;
	App->camera->engine_camera->SetAspectRatio((float)width / (float)height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glLoadMatrixf((float*)&App->camera->GetProjectionMatrix());

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}
