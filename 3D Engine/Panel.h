#ifndef __PANEL_H__
#define __PANEL_H__

// Base class for all possible Editor Panels
#include "Globals.h"
#include <string>
#include "SDL/include/SDL_scancode.h"

class Panel
{
public:
	Panel(const char* name);
	virtual ~Panel();

	void Activate();
	bool IsActive() const;

	virtual void Draw() = 0;

public:
	bool active = true;

protected:
	std::string name;
};

#endif // __PANEL_H__