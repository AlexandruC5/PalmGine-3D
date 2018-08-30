#include "GameObject.h"
#include "Globals.h"

GameObject::GameObject(GameObject* parent) : parent(parent)
{
	if (parent != nullptr)
	{
		parent->childs.push_back(this);
	}
}

GameObject::~GameObject()
{
	RELEASE_ARRAY(name);
	parent = nullptr;
	// Clear components
	childs.clear();
}

void GameObject::SetName(char* new_name)
{
	this->name = new_name;
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
}
