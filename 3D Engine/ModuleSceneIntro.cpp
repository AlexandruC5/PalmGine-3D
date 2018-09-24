#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "ModuleConsole.h"

#pragma comment( lib, "Glew/libx86/glew32.lib" )

ModuleSceneIntro::ModuleSceneIntro(bool start_enabled) : Module(start_enabled)
{
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;
	App->camera->Move(vec3(1.0f, 1.0f, 0.0f));
	App->camera->LookAt(vec3(0, 0, 0));

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");
	return true;
}

GameObject * ModuleSceneIntro::AddGameObject(GameObject* parent)
{
	GameObject* go = new GameObject(parent);
	gameObjects.push_back(go);
	
	return go;
}



// PreUpdate
update_status ModuleSceneIntro::PreUpdate(float dt)
{
	// TODO Preupdate go
	return(UPDATE_CONTINUE);
}

// Update
update_status ModuleSceneIntro::Update(float dt)
{
	// Render grid
	if (grid_enabled)
	{
		PlaneGrid p(0, 1, 0, 0);
		if (axis_enabled)
		{
			p.axis = true;
		}
		p.Render();
	}

	// TODO update go
	return UPDATE_CONTINUE;
}