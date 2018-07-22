#ifndef __ModuleHardware_H__
#define __ModuleHardware_H__

#include "Module.h"
#include "Globals.h"
#include "imGUI\imgui.h"
#include "Glew\include\glew.h"
#include "SDL/include/SDL_cpuinfo.h"
#include "SDL/include/SDL_opengl.h"

class ModuleHardware : public Module
{
public:
	ModuleHardware(bool start_enabled = false);
	~ModuleHardware();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	void Draw(const char* title);

	bool active = true;
private:

	SDL_version currentSDLVersion;

	const GLubyte* vendor = nullptr;
	const GLubyte* gpuName = nullptr;
	const GLubyte* version = nullptr;

	GLint memoryUsage = 0;
	GLint dedicatedMemory = 0;
	GLint totalMemory = 0;
	GLint availableMemory = 0;

	//------

	int cpus = 0;
	int cache = 0;
	int systemRam = 0;

	bool avx = false;
	bool rdtsc = false;
	bool mmx = false;
	bool sse = false;
	bool sse2 = false;
	bool sse3 = false;
	bool sse41 = false;
	bool sse42 = false;

};

#endif 
