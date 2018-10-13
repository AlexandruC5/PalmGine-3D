#ifndef MODULEUI_H
#define MODULEUI_H

#include "Module.h"
#include "Globals.h"

class Panel;
class PanelAbout;

class ModuleUI : public Module
{
public:
	ModuleUI(bool start_enabled = true);
	~ModuleUI();

	bool Start();
	update_status Update(float dt);
	update_status PreUpdate(float dt);
	bool CleanUp();

private:
	bool configActive = true;

public:
	PanelAbout* about = nullptr;
};

#endif // !MODULEUI_H