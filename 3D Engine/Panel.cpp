#include "Panel.h"

Panel::Panel(const char* name) : name(name)
{}

Panel::~Panel()
{}

void Panel::Activate()
{
	active = !active;
}

bool Panel::IsActive() const
{
	return active;
}