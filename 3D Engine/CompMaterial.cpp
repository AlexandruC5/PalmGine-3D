#include "CompMaterial.h"
#include "GameObject.h"
#include "ResourceTexture.h"

CompMaterial::CompMaterial(GameObject * parent, COMP_TYPE type) : Component(parent, type)
{
	parent->AddComponent(this);
}

CompMaterial::~CompMaterial()
{
	rtexture->texture->texture_path.clear();
	rtexture->already_loaded--;
	if (rtexture->already_loaded <= 0)
		RELEASE(rtexture);
}

void CompMaterial::SetID(uint id, std::string path, int width, int height)
{
	rtexture->texture->id = id;
	rtexture->texture->texture_path = path;
	rtexture->texture->texture_width = width;
	rtexture->texture->texture_height = height;
}

uint CompMaterial::GetTextureId() const
{
	return rtexture->texture->id;
}

std::string CompMaterial::GetTexturePath() const 
{
	return rtexture->texture->texture_path;
}

int CompMaterial::GetTextureWidth()const 
{
	return rtexture->texture->texture_width;
}

int CompMaterial::GetTextureHeight()const 
{
	return rtexture->texture->texture_height;
}

uint CompMaterial::GetSize() const
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
	strcpy(name, rtexture->exported_path.c_str());
	memcpy(cursor[0], name, bytes);
	cursor[0] += bytes;
}
