#ifndef MODULESCENEINTRO_H
#define MODULESCENEINTRO_H


#include "Module.h"
#include "Globals.h"

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update(float dt);
	update_status PreUpdate(float dt);
	bool CleanUp();

	// Variables
	bool grid_enabled = true;
	bool axis_enabled = false;
};


#endif // !MODULESCENEINTRO_H