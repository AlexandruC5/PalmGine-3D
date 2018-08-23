#pragma once
#include "Globals.h"
#include <vector>

class GameObject
{
public:
	GameObject(GameObject* parent);
	~GameObject();

	// Name utility
	void SetName(char* new_name);
	const char* GetName() const;

	// State
	void SetActive();
	bool IsActive();

	// Parent
	const GameObject* GetParent() const;

	// Childs
	uint GetNumChilds() const;

private:

	uint id = 0;
	char* name = "default";
	bool active = true;
	bool static_obj = false;

	GameObject* parent = nullptr;
	std::vector<GameObject*> childs;
	// TODO: add the component's vector :)
};