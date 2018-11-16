#ifndef MODULESCENEINTRO_H
#define MODULESCENEINTRO_H


#include "Module.h"
#include "Globals.h"
#include "GameObject.h"
#include "QuadTree.h"
#include "InGameTimer.h"

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update(float dt);
	update_status PreUpdate(float dt);
	bool CleanUp();
	GameObject* AddGameObject(GameObject* parent);

	// Variables
	bool grid_enabled = true;
	bool axis_enabled = false;
	
	// Utility
	bool IsRootGO(const GameObject* go) const;
	void SetSelectedGameObject(const GameObject* target);
	GameObject* GetSelectedGO() const;
	void SetGameObjectDrawability();

	void PickGO(const LineSegment picker);
	GameObject* TestRayWithAllGO(const LineSegment& picker) const;
	void TestGOOutOfQuad(std::vector<GameObject*> &posible_GOs_picked, GameObject* posibleGO) const;

	// Binary Serialization
	void SerializeScene(const char* name);
	uint GetSceneSize(GameObject* go, uint* go_num);
	uint GetGameObjectSceneSize(GameObject* go);
	void CreateData(char** cursor, GameObject* go);
	void CreateFileData(const char* name, char* data, uint size);

	void LoadSceneData(const char* path);
	char* ReadBinaryScene(const char*path);

public:

	GameObject* root_gameObjects =  nullptr;
	GameObject* selected_gameObject = nullptr;
	GameObject* camera = nullptr;
	Quadtree quadtree;
	char* last_scene_name = nullptr;

	InGameTimer in_game_timer;
	bool game_running = false;
	bool game_paused = false;
	float in_game_time_scale = 1.0f;

	float in_game_dt;
	std::vector<GameObject*> to_delete;
};

#endif // !MODULESCENEINTRO_H
