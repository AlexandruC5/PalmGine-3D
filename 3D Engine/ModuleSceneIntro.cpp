#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "CompCamera.h"
#include "CompTransform.h"
#include "CompMesh.h"
#include "Component.h"
#include "PanelLoadScene.h"
#include "MathGeoLib/MathGeoLib.h"
#include <list>
#include "Devil/include/il.h"
#include "Devil/include/ilut.h"

#pragma comment( lib, "Glew/libx86/glew32.lib" )

ModuleSceneIntro::ModuleSceneIntro(bool start_enabled) : Module(start_enabled)
{
	in_game_timer.Start();
	in_game_timer.Stop();
}

ModuleSceneIntro::~ModuleSceneIntro()
{
	quadtree.~Quadtree();
	RELEASE(root_gameObjects);
	
	for (uint i = 0; i < to_delete.size(); i++)
	{
		delete to_delete[i];
	}
}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");

	JSONscene_obj = json_object_get_object(App->JSONconfig_obj, "scene");
	game_running = json_object_get_boolean(JSONscene_obj, "inGame");

	bool ret = true;
	App->camera->LookAt(float3::zero);
	
	root_gameObjects = new GameObject(nullptr);
	root_gameObjects->SetName("Root");
	quadtree.SetBoundries(AABB(float3(-500, 0, -500), float3(500, 30, 500)));
	camera = new GameObject(root_gameObjects);
	camera->SetName("MainCamera");
	CompCamera* cameracomp = new CompCamera(camera, COMP_TYPE::C_CAMERA);
	camera->AddComponent(cameracomp);

	if (game_running == true) {
		in_game_timer.Start();
		SerializeScene("AutoSaveWhenPlay");
	}

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");
	JSONscene_obj = nullptr;
	return true;
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
	in_game_timer.UpdateTimer();
	in_game_dt = in_game_timer.GetDT();

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

	root_gameObjects->Update(in_game_dt);

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
		else {
			for (int i = 0; i < root_gameObjects->GetNumChilds(); ++i) {
				for (int j = 0; j < root_gameObjects->childs[i]->GetNumChilds(); ++j) {
					if (root_gameObjects->childs[i]->childs[j]->IsStatic() == true) {
						if (root_gameObjects->childs[i]->childs[j]->GetCompMesh() != nullptr) {
							root_gameObjects->childs[i]->childs[j]->GetCompMesh()->Draw();
						}
					}
				}
				if (root_gameObjects->childs[i]->IsStatic() == true) {
					if (root_gameObjects->childs[i]->GetCompMesh() != nullptr) {
						root_gameObjects->childs[i]->GetCompMesh()->Draw();
					}
				}
			}
		}
	}
}

void ModuleSceneIntro::PickGO(const LineSegment picker) 
{
	selected_gameObject = TestRayWithAllGO(picker);
}

GameObject* ModuleSceneIntro::TestRayWithAllGO(const LineSegment& picker) const 
{
	GameObject* posibleGO = nullptr;
	std::vector<GameObject*> posible_GOs_picked;
	quadtree.CollectIntersections(posible_GOs_picked, picker);

	float lower_hit_distance = inf;

	for (uint i = 0; i < root_gameObjects->GetNumChilds(); ++i) 
	{
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
					nearestTriangle.a = { GOMesh->vertices[GOMesh->indices[iterator] * 3], GOMesh->vertices[GOMesh->indices[iterator] * 3 + 1], GOMesh->vertices[GOMesh->indices[iterator] * 3 + 2] };
					nearestTriangle.b = { GOMesh->vertices[GOMesh->indices[iterator] * 3 + 3], GOMesh->vertices[GOMesh->indices[iterator] * 3 + 4], GOMesh->vertices[GOMesh->indices[iterator] * 3 + 5] };
					nearestTriangle.c = { GOMesh->vertices[GOMesh->indices[iterator] * 3 + 6], GOMesh->vertices[GOMesh->indices[iterator] * 3 + 7], GOMesh->vertices[GOMesh->indices[iterator] * 3 + 8] };

					float3 hit_point = float3::zero;
					float hit_distance = 0.0f;

					if (picker_in_local.Intersects(nearestTriangle, &hit_distance, &hit_point)) {
						posibleGO = posible_GOs_picked[i];
					}
				}
			}
		}
	}
	return posibleGO;
}

void ModuleSceneIntro::TestGOOutOfQuad(std::vector<GameObject*> &posible_GOs_picked, GameObject* posibleGO) const 
{
	if (posibleGO->IsActive() == true && posibleGO->IsStatic() == false) 
	{
		if (App->camera->engine_camera->frustum.Contains(posibleGO->GetAABB()) == true) 
		{
			posible_GOs_picked.push_back(posibleGO);
		}
		for (uint i = 0; i <posibleGO->GetNumChilds(); ++i) {
			TestGOOutOfQuad(posible_GOs_picked, posibleGO->childs[i]);
		}
	}
}

void ModuleSceneIntro::SerializeScene(const char * name)
{
	char* data = nullptr;
	char* cursor = nullptr;
	uint size = sizeof(uint); // First, the num of GO we will have
	uint go_num = 0;
	uint bytes = 0;

	size += GetSceneSize(root_gameObjects, &go_num);
	data = new char[size];
	cursor = data;
	
	// Number of GameObjects
	bytes = sizeof(uint);
	memcpy(cursor, &go_num, bytes);
	cursor += bytes;

	CreateData(&cursor, root_gameObjects);

	CreateFileData(name, data, size);

	LOG("SUCCESS SAVING SCENE");
}

uint ModuleSceneIntro::GetSceneSize(GameObject* go, uint* go_num)
{
	uint size = 0;
	go_num[0] += 1;
	size = GetGameObjectSceneSize(go);
	for (uint i = 0; i < go->components.size(); i++)
	{
		switch (go->components[i]->GetType())
		{
		case COMP_TYPE::C_TRANSFORM:
		{
			size += ((CompTransform*)go->components[i])->GetSize();
			break;
		}
		case COMP_TYPE::C_MESH:
		{
			size += ((CompMesh*)go->components[i])->GetSize();
			break;
		}
		case COMP_TYPE::C_MATERIAL:
		{
			size += ((CompMaterial*)go->components[i])->GetSize();
			break;
		}
		case COMP_TYPE::C_CAMERA:
		{
			size += ((CompCamera*)go->components[i])->GetSize();
			break;
		}
		}
	}
	for (uint i = 0; i < go->GetNumChilds(); i++)
	{
		size += GetSceneSize(go->childs[i], go_num);
	}
	return size;
}

uint ModuleSceneIntro::GetGameObjectSceneSize(GameObject* go)
{
	uint size = 0;

	// UUID 
	size += sizeof(uint);
	// UUID PARENT
	size += sizeof(uint);
	// NAME
	size += sizeof(char)*64;
	// ACTIVE
	size += sizeof(int);
	// STATIC
	size += sizeof(int);
	// NUM COMPONENTS
	size += sizeof(uint);

	for (uint i = 0; i < go->components.size(); i++)
	{
		size += go->components[i]->GetSize();
	}
	return size;
}

void ModuleSceneIntro::CreateData(char ** cursor, GameObject * go)
{
	uint bytes = 0;

	// UUID 
	bytes = sizeof(uint);
	uint uuid = go->GetUUID();
	memcpy(cursor[0], &uuid, bytes);
	cursor[0] += bytes;
	// UUID PARENT
	bytes = sizeof(uint);
	uint tmp_uuid = 0;
	if (go->GetParent() != nullptr)
		tmp_uuid = go->GetParent()->GetUUID();	
	memcpy(cursor[0], &tmp_uuid, bytes);	
	cursor[0] += bytes;
	// NAME
	bytes = sizeof(char) * 64;
	char* name = new char[64];
	strcpy(name, go->GetName().c_str());
	memcpy(cursor[0], name, bytes);
	cursor[0] += bytes;
	// ACTIVE
	bytes = sizeof(int);
	int is_active = ((int)go->IsActive());
	memcpy(cursor[0], &is_active, bytes);
	cursor[0] += bytes;
	// STATIC
	bytes = sizeof(int);
	int is_static = ((int)go->IsStatic());
	memcpy(cursor[0], &is_static, bytes);
	cursor[0] += bytes;
	// NUM COMPONENTS
	bytes = sizeof(uint);
	uint num_comps = (uint)go->components.size();
	memcpy(cursor[0], &num_comps, bytes);
	cursor[0] += bytes;

	for (uint i = 0; i < go->components.size(); i++)
	{
		switch (go->components[i]->GetType())
		{
		case COMP_TYPE::C_TRANSFORM:
		{
			((CompTransform*)go->components[i])->WriteComponentData(cursor);
			break;
		}
		case COMP_TYPE::C_MATERIAL:
		{
			((CompMaterial*)go->components[i])->WriteComponentData(cursor);
			break;
		}
		case COMP_TYPE::C_MESH:
		{
			((CompMesh*)go->components[i])->WriteComponentData(cursor);
			break;
		}
		case COMP_TYPE::C_CAMERA:
		{
			((CompCamera*)go->components[i])->WriteComponentData(cursor);
			break;
		}
		}
	}
	for (uint i = 0; i < go->childs.size(); i++)
	{
		CreateData(cursor, go->childs[i]);
	}
}

void ModuleSceneIntro::CreateFileData(const char * name, char* data, uint size)
{
	std::string bin_path;
	FILE * pFile;
	bin_path = BINARY_SCENE_PATH;
	bin_path += name;
	bin_path += ".binaryscene";
	if (!FileExist(bin_path.c_str()))
	{
		pFile = fopen(bin_path.c_str(), "wb");
		fwrite(data, sizeof(char), size, pFile);
		fclose(pFile);
		App->ui->load_scene->scenes.push_back(name);
	}
	else
	{
		DeleteFileA(bin_path.c_str());
		pFile = fopen(bin_path.c_str(), "wb");
		fwrite(data, sizeof(char), size, pFile);
		fclose(pFile);
	}
}

void ModuleSceneIntro::LoadSceneData(const char * path)
{
	std::string bin_path;
	FILE * pFile;
	bin_path = BINARY_SCENE_PATH;
	bin_path += path;
	bin_path += ".binaryscene";
	char* data = ReadBinaryScene(bin_path.c_str());
	char* cursor = data;
	uint bytes = 0;
	uint num_go = 0;
	std::vector<GameObject*> go_list;
	
	bytes = sizeof(uint);
	memcpy(&num_go, cursor, bytes);
	cursor += bytes;

	for (uint i = 0; i < num_go; i++)
	{
		uint uuid = 0;
		uint parent_uuid = 0;
		char* name = new char[64];
		int is_active = 0;
		int is_static = 0;
		uint num_comp = 0;
		GameObject* go = new GameObject(nullptr);

		// UUID
		bytes = sizeof(uint);
		memcpy(&uuid, cursor, bytes);
		go->SetUUID(uuid);
		cursor += bytes;
		// PARENT UUID
		bytes = sizeof(uint);
		memcpy(&parent_uuid, cursor, bytes);
		go->SetParentUUID(parent_uuid);
		cursor += bytes;
		// NAME
		bytes = sizeof(char)*64;
		memcpy(name, cursor, bytes);
		go->SetName(name);
		cursor += bytes;
		// IS ACTIVE
		bytes = sizeof(int);
		memcpy(&is_active, cursor, bytes);
		go->SetActive((bool)is_active);
		cursor += bytes;
		// IS STATIC
		bytes = sizeof(int);
		memcpy(&is_static, cursor, bytes);
		go->SetStatic((bool)is_static);
		cursor += bytes;
		// NUM COMPONENTS
		bytes = sizeof(uint);
		memcpy(&num_comp, cursor, bytes);
		cursor += bytes;

		for (uint j = 0; j < num_comp; j++)
		{
			int comp_type = 0;
			int comp_active = 0;

			// COMPONENT TYPE
			bytes = sizeof(int);
			memcpy(&comp_type, cursor, bytes);
			cursor += bytes;
			// IS ACTIVE
			bytes = sizeof(int);
			memcpy(&comp_active, cursor, bytes);
			cursor += bytes;

			switch (comp_type)
			{
			case COMP_TYPE::C_TRANSFORM:
			{
				CompTransform* comp_trans = go->GetCompTransform();
				float3 translation = { 0,0,0 };
				float3 rotation = { 0,0,0 };
				float3 scale = { 0,0,0 };
				// TRANSLATION
				bytes = sizeof(float3);
				memcpy(&translation, cursor, bytes);
				cursor += bytes;
				// ROTATION
				bytes = sizeof(float3);
				memcpy(&rotation, cursor, bytes);
				cursor += bytes;
				// SCALE
				bytes = sizeof(float3);
				memcpy(&scale, cursor, bytes);
				cursor += bytes;
				// set values to component
				comp_trans->SetPosition(translation);
				comp_trans->SetRotation(rotation);
				comp_trans->SetScale(scale);
				break;
			}
			case COMP_TYPE::C_MESH:
			{
				char* path = new char[128];
				// PATH
				bytes = sizeof(char) * 128;
				memcpy(path, cursor, bytes);
				cursor += bytes;
			
				uint mesh_uuid = App->resource_manager->SearchResource(path);

				if (mesh_uuid == 0)
					App->importer->ReadBinaryMesh(path, go);
				else
				{
					CompMesh* mesh_comp = new CompMesh(go, COMP_TYPE::C_MESH);
					mesh_comp->rmesh = (ResourceMesh*)App->resource_manager->resources[mesh_uuid];
					mesh_comp->rmesh->already_loaded++;
				}
			
				break;
			}
			case COMP_TYPE::C_MATERIAL:
			{
				char* path = new char[128];
				// PATH
				bytes = sizeof(char) * 128;
				memcpy(path, cursor, bytes);
				cursor += bytes;

				// Resource texture
				uint texture_uuid = App->resource_manager->SearchResource(path);

				if (texture_uuid == 0)
					App->importer->LoadDDS(path, go);
				else
				{
					CompMaterial* mat_comp = new CompMaterial(go, COMP_TYPE::C_MATERIAL);
					mat_comp->rtexture = (ResourceTexture*)App->resource_manager->resources[texture_uuid];
					mat_comp->SetID(mat_comp->rtexture->texture->id, mat_comp->rtexture->exported_path, ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT));
					mat_comp->rtexture->already_loaded++;
				}
				break;
			}
			case COMP_TYPE::C_CAMERA:
			{
				CompCamera* comp_camera = new CompCamera(go, COMP_TYPE::C_CAMERA);
				Frustum frustum = Frustum();
				int frustum_culling = 0;
				// FRUSTRUM
				bytes = sizeof(Frustum);
				memcpy(&frustum, cursor, bytes);
				cursor += bytes;
				// FRUSTRUM CULLING ACTIVE
				bytes = sizeof(int);
				memcpy(&frustum_culling, cursor, bytes);
				cursor += bytes;
				// set values to component
				comp_camera->frustum = frustum;
				comp_camera->frustum_culling = frustum_culling;
				go->AddComponent(comp_camera);
				// Set as scene camera
				if(go->IsActive())
					camera = go;
				break;
			}
			}	
		}
		go_list.push_back(go);
	}
	
	// Fill childs
	delete root_gameObjects;
	selected_gameObject = nullptr;

	for (uint i = 0; i < go_list.size(); i++)
	{
		if (go_list[i]->GetParentUUID() == 0)
		{
			//root_gameObjects->AddChild(go_list[i]);
			root_gameObjects = go_list[i];
		}
		uint current_uuid = go_list[i]->GetUUID();
		for (uint j = 0; j < go_list.size(); j++)
		{
			if (go_list[j]->GetParentUUID() == current_uuid)
			{
				go_list[i]->AddChild(go_list[j]);
			}
		}
	}
	LOG("LOADED SCENE WITH NAME: %s", path);
}

char * ModuleSceneIntro::ReadBinaryScene(const char * path)
{
	FILE * pFile;
	long lSize;
	char * data;
	size_t result;

	pFile = fopen(path, "rb");
	if (pFile == NULL) { fputs("File error", stderr); exit(1); }

	//Obtain file size:
	fseek(pFile, 0, SEEK_END);
	lSize = ftell(pFile);
	rewind(pFile);

	//Allocate memory to contain the whole file:
	data = new char[lSize];// (char*)malloc(sizeof(char)*lSize);
	if (data == NULL) { fputs("Memory error", stderr); exit(2); }

	//Copy the file into the buffer:
	result = fread(data, 1, lSize, pFile);
	if (result != lSize) { fputs("Reading error", stderr); exit(3); }

	fclose(pFile);

	return data;
}

