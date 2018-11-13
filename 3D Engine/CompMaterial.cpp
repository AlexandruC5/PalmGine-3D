#include "CompMaterial.h"
#include "GameObject.h"

CompMaterial::CompMaterial(GameObject * parent, COMP_TYPE type) : Component(parent, type)
{
	texture = new Texture();
	parent->AddComponent(this);
}

CompMaterial::~CompMaterial()
{
	texture->texture_path.clear();
	delete texture;
}

void CompMaterial::SetID(uint id, std::string path, int width, int height)
{
	texture->id = id;
	texture->texture_path = path;
	texture->textureWidth = width;
	texture->textureHeight = height;
}

uint CompMaterial::GetTextureId() const
{
	return texture->id;
}

std::string CompMaterial::GetTexturePath() const {
	return texture->texture_path;
}

int CompMaterial::GetTextureWidth()const {
	return texture->textureWidth;
}

int CompMaterial::GetTextureHeight()const {
	return texture->textureHeight;
}

uint CompMaterial::GetSize() 
{
	// GET SIZE FOR SERIALIZATION
	uint size = 0;

	// COMPONENT TYPE
	size += sizeof(int);
	// IS ACTIVE
	size += sizeof(int);
	// BINARY PATH
	size += sizeof(char)*128;

	return size;
}

void CompMaterial::WriteComponentData(char ** cursor)
{
	uint bytes = 0;

	// COMPONENT TYPE
	bytes = sizeof(int);
	memcpy(cursor[0], &type, bytes);
	cursor[0] += bytes;
	// IS ACTIVE
	bytes = sizeof(int);
	int tmp_active = (int)active;
	memcpy(cursor[0], &tmp_active, bytes);
	cursor[0] += bytes;
	// BINARY PATH
	bytes = sizeof(char) * 128;
	char* name = new char[128];
	strcpy(name, binary_path);
	memcpy(cursor[0], name, bytes);
	cursor[0] += bytes;
}
