#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "CompCamera.h"
#include "CompTransform.h"
#include "CompMesh.h"
#include "Component.h"
#include "MathGeoLib/MathGeoLib.h"

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
				(*iterator).second->GetCompMesh()->Draw();
			}
		}
	}
}

void ModuleSceneIntro::PickGO(const LineSegment picker) {
	//Test GO to see which one is the one that the mouse is picking
	selected_gameObject = TestRayWithAllGO(picker);
}

GameObject* ModuleSceneIntro::TestRayWithAllGO(const LineSegment& picker) const {
	GameObject* posibleGO = nullptr;
	std::vector<GameObject*> posible_GOs_picked;
	quadtree.CollectIntersections(posible_GOs_picked, picker);

	float lower_hit_distance = inf;

	for (uint i = 0; i < root_gameObjects->GetNumChilds(); ++i) {
		TestGOOutOfQuad(posible_GOs_picked, root_gameObjects->childs[i]);
	}

	Mesh* GOMesh = nullptr;
	CompMesh* GOCompMesh = nullptr;
	CompTransform* GOTrans = nullptr;

	for (uint i = 0; i < posible_GOs_picked.size(); ++i) {
		math::Triangle nearestTriangle;
		math::LineSegment picker_in_local(picker);

		GOTrans = posible_GOs_picked[i]->GetCompTransform();

		if (GOTrans != nullptr) {
			//Pass ray to localPos
			picker_in_local.Transform(GOTrans->GetTransformationMatrix().Inverted());

			GOCompMesh = posible_GOs_picked[i]->GetCompMesh();

			if (GOCompMesh != nullptr) {
				GOMesh = GOCompMesh->GetMesh();

				for (uint iterator = 0; iterator < GOMesh->num_indices; ++iterator) {
					//Set triangle
					nearestTriangle.a = { GOMesh->vertices[GOMesh->indices[iterator] * 3], GOMesh->vertices[GOMesh->indices[iterator] * 3 + 1], GOMesh->vertices[GOMesh->indices[iterator] * 3 + 2] };
					iterator++;
					nearestTriangle.b = { GOMesh->vertices[GOMesh->indices[iterator] * 3], GOMesh->vertices[GOMesh->indices[iterator] * 3 + 1], GOMesh->vertices[GOMesh->indices[iterator] * 3 + 2] };
					iterator++;
					nearestTriangle.c = { GOMesh->vertices[GOMesh->indices[iterator] * 3], GOMesh->vertices[GOMesh->indices[iterator] * 3 + 1], GOMesh->vertices[GOMesh->indices[iterator] * 3 + 2] };
					iterator++;

					float3 hit_point = float3::zero;
					float hit_distance = 0.0f;

					if (picker_in_local.Intersects(nearestTriangle, &hit_distance, &hit_point)) {
						if (hit_distance < lower_hit_distance) {
							lower_hit_distance = hit_distance;
							posibleGO = posible_GOs_picked[i];
						}
					}
				}
			}
		}
	}
	return posibleGO;
}

//Used to insert GO that aren't in quadtree but are in frustrum
void ModuleSceneIntro::TestGOOutOfQuad(std::vector<GameObject*> &posible_GOs_picked, GameObject* posibleGO) const {
	if (posibleGO->IsActive() == true && posibleGO->IsStatic() == false) {
		if (App->camera->engine_camera->frustum.Contains(posibleGO->GetAABB()) == true) {
			posible_GOs_picked.push_back(posibleGO);
		}

		for (uint i = 0; i <posibleGO->GetNumChilds(); ++i) {
			TestGOOutOfQuad(posible_GOs_picked, posibleGO->childs[i]);
		}
	}
}

