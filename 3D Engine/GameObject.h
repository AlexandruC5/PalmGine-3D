#pragma once
#include "Globals.h"
#include "Component.h"
#include "CompMesh.h"
#include "CompMaterial.h"
#include "CompTransform.h"
#include <vector>

class GameObject
{
public:
	GameObject(GameObject* parent);
	~GameObject();

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
	uint GetNumChilds() const;

	// Components ---------------------
	CompMesh* GetCompMesh() const;
	CompTransform* GetCompTransform() const;
	CompMaterial* GetCompMaterial() const;
	Component* FindComponent(COMP_TYPE type) const;
private:

	uint id = 0;
	char* name = "default";
	bool active = true;
	bool static_obj = false;

	GameObject* parent = nullptr;
	std::vector<GameObject*> childs;
	std::vector<Component*> components;
};