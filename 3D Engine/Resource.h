#ifndef RESOURCE_H
#define RESOURCE_H

#include "Globals.h"

enum ResourceType
{
	R_MESH,
	R_TEXTURE
};

class Resource 
{
public:
	Resource();
	Resource(uint uuid);
	~Resource();

public:
	uint uuid = 0;
	std::string origin_path;
	std::string exported_path;
	uint already_loaded = 0;
	ResourceType type;
};

#endif 