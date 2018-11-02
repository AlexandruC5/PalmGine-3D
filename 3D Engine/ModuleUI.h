#ifndef MODULEUI_H
#define MODULEUI_H

#include "Module.h"
#include "Globals.h"

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
};

#endif // !MODULEUI_H