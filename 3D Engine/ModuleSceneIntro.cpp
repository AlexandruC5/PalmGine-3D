#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "CompCamera.h"
#include "CompTransform.h"
#include "Component.h"

#pragma comment( lib, "Glew/libx86/glew32.lib" )

ModuleSceneIntro::ModuleSceneIntro(bool start_enabled) : Module(start_enabled)
{
}

ModuleSceneIntro::~ModuleSceneIntro()
{
	quadtree.~Quadtree();
	//delete camera;
	delete root_gameObjects;
}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;
	App->camera->Move(vec3(1.0f, 1.0f, 0.0f));
	App->camera->LookAt(vec3(0, 0, 0));
	
	root_gameObjects = new GameObject(nullptr);
	quadtree.SetBoundries(AABB(float3(-500, 0, -500), float3(500, 30, 500)));
	camera = new GameObject(root_gameObjects);
	camera->SetName("MainCamera");
	CompCamera* cameracomp = new CompCamera(camera, COMP_TYPE::C_CAMERA);
	camera->AddComponent(cameracomp);


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
	root_gameObjects->AddChild(go);
	
	return go;
}

bool ModuleSceneIntro::IsRootGO(const GameObject* go) const
{
	return go == root_gameObjects;
}

void ModuleSceneIntro::SetSelectedGameObject(const GameObject * target)
{
	selected_gameObject = (GameObject*)target;
}

GameObject * ModuleSceneIntro::GetSelectedGO() const
{
	return selected_gameObject;
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

	quadtree.DebugDraw();

	return UPDATE_CONTINUE;
}