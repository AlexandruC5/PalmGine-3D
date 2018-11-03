#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "Globals.h"
#include "Component.h"
#include <vector>

class Component;
class CompMesh;
class CompMaterial;
class CompTransform;

class GameObject
{
public:
	GameObject(GameObject* parent);
	~GameObject();

	void Update(float dt);

	// Name --------------------------
	void SetName(const char* new_name);

	// State --------------------------
	bool Enable();
	bool Disable();
	bool IsActive();
	bool IsRootGo();

	// Parent -------------------------
	const GameObject* GetParent() const;

	// Childs -------------------------
	void AddChild(GameObject* child);
	uint GetNumChilds() const;

	// Components ---------------------
	Component* FindComponent(COMP_TYPE type) const;
	bool CompAlreadyExists(COMP_TYPE type) const;
	CompMesh* GetCompMesh() const;
	CompTransform* GetCompTransform() const;
	CompMaterial* GetCompMaterial() const;
	Component* AddEmptyComponent(COMP_TYPE type);
	void AddComponent(Component* comp);

	// UI
	void BlitGameObjectHierarchy();
	void BlitGameObjectInspector();

private:
	uint id = 0;
	std::string name = "Unnamed";
	bool active = true;
	bool static_obj = false;

	GameObject* parent = nullptr;
	std::vector<GameObject*> childs;
	std::vector<Component*> components;
};

#endif // !GAMEOBJECT_H