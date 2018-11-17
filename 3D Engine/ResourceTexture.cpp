#include "ResourceTexture.h"
#include "CompMaterial.h"

ResourceTexture::ResourceTexture()
{
	uuid = GenRandomNumber();
	texture = new Texture();
}

ResourceTexture::ResourceTexture(uint uuid)
{}

ResourceTexture::~ResourceTexture()
{
	delete texture;
}
