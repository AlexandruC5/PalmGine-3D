#include "ResourceMesh.h"
#include "CompMesh.h"
#include "Globals.h"

ResourceMesh::ResourceMesh(uint uuid)
{}

ResourceMesh::ResourceMesh()
{
	uuid = GenRandomNumber();
}

ResourceMesh::~ResourceMesh()
{
	RELEASE_ARRAY(mesh->indices);
	RELEASE_ARRAY(mesh->vertices);
	RELEASE_ARRAY(mesh->normals);
	RELEASE_ARRAY(mesh->uvs);
	RELEASE(mesh);
}
