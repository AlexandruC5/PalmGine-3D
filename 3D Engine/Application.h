#ifndef APPLICATON_H
#define APPLICATON_H

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
#include "ModuleHardware.h"
#include "ModuleUI.h"
#include "ModuleImporter.h"
#include "ModuleResourceManager.h"

class Application
{
public:
	ModuleWindow* window;
	ModuleInput* input;
	ModuleHardware* hardware;
	ModuleSceneIntro* scene_intro;
	ModuleRenderer3D* renderer3D;
	ModuleCamera3D* camera;
	ModuleUI* ui;
	ModuleImporter* importer;
	ModuleResourceManager* resource_manager;
	JSON_Value* JSONconfig = nullptr;
	JSON_Object* JSONconfig_obj = nullptr;

	Timer	ms_timer;

private:

	//Timer	ms_timer;
	float	dt;
	std::list<Module*> list_modules;

public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();
	float GetDT() const;

private:

	void AddModule(Module* mod);
	void PrepareUpdate();
	void FinishUpdate();
};

extern Application* App;

#endif // !APPLICATON_H