#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "CompCamera.h"
#include "CompTransform.h"
#include "CompMesh.h"
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
	//App->camera->Move(vec3(1.0f, 1.0f, 0.0f));
	App->camera->LookAt(float3::zero);
	
	root_gameObjects = new GameObject(nullptr);
	root_gameObjects->SetName("Root");
	quadtree.SetBoundries(AABB(float3(-500, 0, -500), float3(500, 30, 500)));
	camera = new GameObject(root_gameObjects);
	camera->SetName("MainCamera");
	CompCamera* cameracomp = new CompCamera(camera, COMP_TYPE::C_CAMERA);
	camera->AddComponent(cameracomp);
	//TODO Add AABB to all game objects and uncomment this
	//quadtree.Insert(camera);


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

	SetGameObjectDrawability();

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::SetGameObjectDrawability() {

	if (quadtree.root == nullptr) {
		//FrustumCulling without quadtree
		if (camera->GetCompCamera()->frustum_culling == true) {
			for (int i = 0; i < root_gameObjects->GetNumChilds(); ++i) {
				if (root_gameObjects->childs[i]->IsActive() == true && root_gameObjects->childs[i]->IsStatic() == true) {
					if (root_gameObjects->childs[i]->GetCompMesh() != nullptr) {
						if (camera->GetCompCamera()->frustum.Intersects(root_gameObjects->childs[i]->GetAABB())) {
							root_gameObjects->childs[i]->GetCompMesh()->Draw();
						}
						//else {
						//	root_gameObjects->childs[i]->GetCompMesh()->drawable = false;
						//}
					}
				}
				else if (root_gameObjects->childs[i]->GetCompMesh() != nullptr) {
					root_gameObjects->childs[i]->GetCompMesh()->Draw();
				}
			}
		}
	}
	else {
		//FrustumCulling with quadtree
		if (camera->GetCompCamera()->frustum_culling == true) {
			std::map<float, GameObject*> objects_colliding;
			quadtree.CollectIntersections(objects_colliding, camera->GetCompCamera()->frustum);
			for (std::map<float, GameObject*>::reverse_iterator iterator = objects_colliding.rbegin(); iterator != objects_colliding.rend(); ++iterator) {
				//TODO finish frustum culling with quadtree
				(*iterator).second->GetCompMesh()->Draw();
			}
		}
	}


}