#ifndef __ModuleAbout_H__
#define __ModuleAbout_H__

#include "Module.h"
#include "Globals.h"
#include "imGUI\imgui.h"
#include "Glew\include\glew.h"
#include "SDL\include\SDL_cpuinfo.h"
#include "SDL\include\SDL_opengl.h"

class ModuleAbout : public Module
{
public:
	ModuleAbout(bool start_enabled = false);
	~ModuleAbout();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	void Draw();

	bool active = true;
private:


};

#endif 
