#include "Component.h"
#include "Application.h"

Component::Component(GameObject * parent, COMP_TYPE type) : parent(parent), type(type)
{}

Component::~Component()
{}

bool Component::Enable()
{
	return active = true;
}

bool Component::Disable()
{
	return active = false;
}

void Component::preUpdate(float dt)
{}

void Component::Update(float dt)
{}

void Component::Draw()
{}

void Component::Clear()
{}

COMP_TYPE Component::GetType() const
{
	return type;
}

bool Component::IsActive() const
{
	return active;
}

const char * Component::GetName() const
{
	return name;
}

void Component::BlitComponentInspector()
{
	ImGui::Text("Component");
	ImGui::Checkbox("Active", &active);
}