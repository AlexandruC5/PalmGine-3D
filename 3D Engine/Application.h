#ifndef APPLICATION_H
#define APPLICATION_H

#include <list>
#include "Globals.h"
#include "JSON\parson.h"
#include "Timer.h"
#include "Module.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleSceneIntro.h"
#include "ModuleRenderer3D.h"
#include "ModuleCamera3D.h"
#include "ModuleConsole.h"
#include "ModuleHardware.h"
#include "ModuleConfig.h"
#include "ModuleUI.h"
#include "ModuleFBX.h"
#include "ModuleInspector.h"

class Application
{
public:
	ModuleWindow* window;
	ModuleInput* input;
	ModuleConsole* console;
	ModuleHardware* hardware;
	ModuleConfig* config;
	ModuleSceneIntro* scene_intro;
	ModuleRenderer3D* renderer3D;
	ModuleCamera3D* camera;
	ModuleUI* ui;
	ModuleFBX* fbx;
	ModuleInspector* inspector;

	JSON_Value* JSONconfig = nullptr;
	JSON_Object* JSONconfig_obj = nullptr;


private:

	Timer	ms_timer;
	float	dt;
	std::list<Module*> list_modules;

public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();

private:

	void AddModule(Module* mod);
	void PrepareUpdate();
	void FinishUpdate();
};

extern Application* App;


#endif // !APPLICATION_H