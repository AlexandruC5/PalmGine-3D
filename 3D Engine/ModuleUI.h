#ifndef MODULEUI_H
#define MODULEUI_H

#include "Module.h"
#include "Globals.h"

#include "Module.h"
#include "Globals.h"

class Panel;
class PanelAbout;
class PanelConsole;
class PanelConfig;
class PanelInspector;
class PanelHierarchy;
class PanelTimeManagement;

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
	std::vector<Panel *> panels;

public:
	PanelAbout* about = nullptr;
	PanelConsole* console = nullptr;
	PanelConfig* config = nullptr;
	PanelInspector* inspector = nullptr;
	PanelHierarchy* goHierarchy = nullptr;
	PanelTimeManagement* time_management = nullptr;

};

#endif // !MODULEUI_H