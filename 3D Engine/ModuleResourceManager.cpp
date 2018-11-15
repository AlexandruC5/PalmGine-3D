#include "Globals.h"
#include "Application.h"
#include "ModuleResourceManager.h"

ModuleResourceManager::ModuleResourceManager(bool start_enabled)
{
}

ModuleResourceManager::~ModuleResourceManager()
{
}

bool ModuleResourceManager::Init()
{
	return true;
}

update_status ModuleResourceManager::PreUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

update_status ModuleResourceManager::Update(float dt)
{
	return UPDATE_CONTINUE;
}

update_status ModuleResourceManager::PostUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

bool ModuleResourceManager::CleanUp()
{
	return true;
}

uint ModuleResourceManager::SearchResource(std::string path)
{
	for (std::map<uint, Resource*>::iterator it = App->resource_manager->resources.begin(); it != App->resource_manager->resources.end(); ++it)
	{
		if (!strcmp((*it).second->exported_path.c_str(), path.c_str()))
		{
			return (*it).first;
		}
	}
	return 0;
}
