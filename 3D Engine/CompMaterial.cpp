#include "CompMaterial.h"

CompMaterial::CompMaterial(GameObject * parent, COMP_TYPE type) : Component(parent, type)
{
}

CompMaterial::~CompMaterial()
{
}

uint const CompMaterial::GetTextureId()
{
	return texture->id;
}
