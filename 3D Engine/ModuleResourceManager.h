#ifndef MODULE_RESOURCE_MANAGER_H
#define MODULE_RESOURCE_MANAGER_H

#include "Module.h"
#include "Globals.h"
#include <map>

class Resource;
class ModuleResourceManager : public Module
{
public:
	ModuleResourceManager(bool start_enabled = true);
	~ModuleResourceManager();

	bool Init();
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

	uint SearchResource(std::string path);

public:
	std::map<uint, Resource*> resources;
};

#endif 