#ifndef __ModuleInspector_H__
#define __ModuleInspector_H__

#include "Module.h"
#include "Globals.h"
#include "imGUI\imgui.h"
#include "Glew\include\glew.h"
#include "SDL\include\SDL_opengl.h"

class ModuleInspector : public Module
{
public:
	ModuleInspector(bool start_enabled = false);
	~ModuleInspector();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	void Draw(const char* title);

	bool active = true;
private:

};

#endif 
