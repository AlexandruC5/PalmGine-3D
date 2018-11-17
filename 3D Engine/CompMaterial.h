#ifndef COMMATERIAL_H
#define COMMATERIAL_H

#include "Component.h"
#include "Globals.h"
#include "ResourceTexture.h"
#include <vector>
#include "MathGeoLib/MathGeoLib.h"

struct Texture
{
	std::string name;
	uint id;
	std::string texture_path;
	int textureWidth = 0;
	int textureHeight = 0;
};

class GameObject;

class CompMaterial : public Component
{
public:
	CompMaterial(GameObject* parent, COMP_TYPE type);
	~CompMaterial();

	// Set
	void SetID(uint id, std::string path, int width, int height);

	// Read
	uint GetTextureId() const;
	std::string GetTexturePath() const;
	int GetTextureWidth() const;
	int GetTextureHeight() const;
	uint GetSize();
	
	void WriteComponentData(char ** cursor);

public:
	ResourceTexture* rtexture;
};

#endif // !COMMATERIAL_H