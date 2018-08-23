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

void GameObject::SetActive()
{
	if (active)
	{
		active = false;
	}
	else
		active = true;
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
