#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "Globals.h"
#include "Component.h"
#include "CompMaterial.h"
#include <vector>

class Component;
class CompMesh;
class CompMaterial;
class CompTransform;
class CompCamera;

class GameObject
{
public:
	GameObject(GameObject* parent);
	~GameObject();

	void Update(float dt);

	// Name --------------------------
	void SetName(const char* new_name);
	std::string GetName();

	// State --------------------------
	bool Enable();
	bool Disable();
	bool IsActive();
	bool IsRootGo() const;
	bool IsStatic() const;
	void SetStatic(bool isStatic);

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
	CompCamera* GetCompCamera() const;
	Component* AddEmptyComponent(COMP_TYPE type);
	void AddComponent(Component* comp);

	// UI
	void BlitGameObjectHierarchy();
	void BlitGameObjectInspector();

	//DebugDrawBox
	void DebugDrawBox();
	math::AABB GetAABB();

private:
	uint id = 0;
	std::string name = "Unnamed";
	bool active = true;
	bool static_obj = false;

	GameObject* parent = nullptr;
	std::vector<Component*> components;

public:
	std::vector<GameObject*> childs;
};

#endif // !GAMEOBJECT_H