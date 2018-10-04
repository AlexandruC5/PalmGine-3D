#include "CompMaterial.h"

CompMaterial::CompMaterial(GameObject * parent, COMP_TYPE type) : Component(parent, type)
{
	texture = new Texture();
}

CompMaterial::~CompMaterial()
{
	delete texture;
}

void CompMaterial::SetID(uint id)
{
	texture->id = id;
}

uint const CompMaterial::GetTextureId()
{
	return texture->id;
}
