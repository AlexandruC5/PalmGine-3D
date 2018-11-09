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