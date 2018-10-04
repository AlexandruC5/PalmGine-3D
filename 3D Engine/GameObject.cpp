#include "GameObject.h"
#include "Globals.h"
#include "CompMesh.h"
#include "CompTransform.h"
#include "CompMaterial.h"


GameObject::GameObject(GameObject* parent) : parent(parent)
{
	this->AddEmptyComponent(COMP_TYPE::C_TRANSFORM);
	if (parent != nullptr)
	{
		parent->childs.push_back(this);
	}
	name = new char[50];
}

GameObject::~GameObject()
{
	RELEASE_ARRAY(name);
	parent = nullptr;
	for (uint i = 0; i < childs.size(); i++)
	{
		delete childs[i];
	}
	for (uint i = 0; i < components.size(); i++)
	{
		delete components[i];
	}

	childs.clear();
	components.clear();
}

void GameObject::Update(float dt)
{
	for (uint i = 0; i < components.size(); i++)
	{
		components[i]->Update(dt);
	}
	for (uint i = 0; i < childs.size(); i++)
	{
		childs[i]->Update(dt);
	}
}

void GameObject::SetName(char* new_name)
{
	if (strcpy(name, new_name) == 0)
	{
		LOG("NAME IS TOO LONG");
	}
}

const char * GameObject::GetName() const
{
	return this->name;
}

bool GameObject::Enable()
{
	return active = true;
}

bool GameObject::Disable()
{
	return active = false;
}

bool GameObject::IsActive()
{
	return active;
}

const GameObject * GameObject::GetParent() const
{
	return parent;
}

void GameObject::AddChild(GameObject * child)
{
	child->parent = this;
	childs.push_back(child);
}

uint GameObject::GetNumChilds() const
{
	return childs.size();
}

// Components -----------------------

CompTransform* GameObject::GetCompTransform() const
{
	return (CompTransform*)FindComponent(COMP_TYPE::C_TRANSFORM);
}

CompMesh* GameObject::GetCompMesh() const
{
	return (CompMesh*)FindComponent(COMP_TYPE::C_MESH);
}

CompMaterial* GameObject::GetCompMaterial() const
{
	return (CompMaterial*)FindComponent(COMP_TYPE::C_MATERIAL);
}

Component * GameObject::AddEmptyComponent(COMP_TYPE type)
{
	if (CompAlreadyExists(type))
	{
		LOG("COMPONENT ALREADY EXITS ON THE GAME OBJECT.");
		return nullptr;
	}
	if (type == COMP_TYPE::C_MESH)
	{
		LOG("Adding MESH COMPONENT to GameObject.");
		CompMesh* mesh = new CompMesh(this, C_MESH);
		components.push_back(mesh);
		return mesh;
	}
	else if (type == COMP_TYPE::C_TRANSFORM)
	{
		LOG("Adding TRANSFORM COMPONENT to GameObject.");
		CompTransform* transform = new CompTransform(this, C_TRANSFORM);
		components.push_back(transform);
		return transform;
	}
	else if (type == COMP_TYPE::C_MATERIAL)
	{
		LOG("Adding MATERIAL COMPONENT to GameObject.");
		CompMaterial* material = new CompMaterial(this, C_MATERIAL);
		components.push_back(material);
		return material;
	}
	
	return nullptr;
}

void GameObject::AddComponent(Component * comp)
{
	if (comp != nullptr)
	{
		if (CompAlreadyExists(comp->GetType()) == false)
		{
			components.push_back(comp);
		}
	}
}

Component * GameObject::FindComponent(COMP_TYPE type) const
{
	Component* temp_comp = nullptr;
	for (uint i = 0; i < components.size(); i++)
	{
		if (components[i]->GetType() == type)
		{
			temp_comp = components[i];
			return temp_comp;
		}
	}
	return temp_comp;
}

bool GameObject::CompAlreadyExists(COMP_TYPE type) const
{
	// Checks if component already exists on the GameObject
	for (uint i = 0; i < components.size(); i++)
	{
		if (components[i]->GetType() == type)
		{
			return true;
		}
	}
	return false;
}
