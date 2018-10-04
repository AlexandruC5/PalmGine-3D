#pragma once
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
	void SetName(char* new_name);
	const char* GetName() const;

	// State --------------------------
	bool Enable();
	bool Disable();
	bool IsActive();

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

private:
	uint id = 0;
	char* name = nullptr;
	bool active = true;
	bool static_obj = false;

	GameObject* parent = nullptr;
	std::vector<GameObject*> childs;
	std::vector<Component*> components;
};