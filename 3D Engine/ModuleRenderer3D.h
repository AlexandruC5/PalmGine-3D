#ifndef MODULERENDERER3D_H
#define MODULERENDERER3D_H

#include "Module.h"
#include "Globals.h"
#include "MathGeoLib/MathGeoLib.h"
#include "Light.h"
#include "ModuleImporter.h"

#define MAX_LIGHTS 8
class Application;

class ModuleRenderer3D : public Module
{
public:
	ModuleRenderer3D(bool start_enabled = true);
	~ModuleRenderer3D();

	bool Init();
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();
	void OnResize(int width, int height);

public:
	Light lights[MAX_LIGHTS];
	SDL_GLContext context;
};

#endif // !MODULERENDERER3D_H