#ifndef MODULESCENEINTRO_H
#define MODULESCENEINTRO_H


#include "Module.h"
#include "Globals.h"
#include "GameObject.h"
#include "QuadTree.h"

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
	uint GetSceneSize(GameObject* go);
	uint GetGameObjectSceneSize(GameObject* go);
	void CreateData(char** cursor, GameObject* go);
	void CreateFileData(const char* name, char* data, uint size);

	void LoadSceneData(char* path);

public:

	GameObject* root_gameObjects =  nullptr;
	GameObject* selected_gameObject = nullptr;
	GameObject* camera = nullptr;

	Quadtree quadtree;
};

#endif // !MODULESCENEINTRO_H
