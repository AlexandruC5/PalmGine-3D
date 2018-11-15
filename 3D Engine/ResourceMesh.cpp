#include "ResourceMesh.h"

ResourceMesh::ResourceMesh(uint uuid)
{}

ResourceMesh::ResourceMesh()
{
	uuid = GenRandomNumber();
}

ResourceMesh::~ResourceMesh()
{
	RELEASE(mesh);
}
