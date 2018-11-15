#ifndef RESOURCE_TEXTURE_H
#define RESOURCE_TEXTURE_H

#include "Resource.h"
#include "Globals.h"

struct Texture;
class ResourceTexture : public Resource
{
public:
	ResourceTexture();
	ResourceTexture(uint uuid);
	~ResourceTexture();

public:
	Texture* texture;
};

#endif 