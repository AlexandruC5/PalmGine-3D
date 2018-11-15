#ifndef RESOURCE_MESH_H
#define RESOURCE_MESH_H

#include "Resource.h"
#include "Globals.h"

struct Mesh;

class ResourceMesh : public Resource
{
public:
	ResourceMesh(uint uuid);
	ResourceMesh();
	~ResourceMesh();

public:
	Mesh* mesh;
};

#endif 