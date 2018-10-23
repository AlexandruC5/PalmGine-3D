#ifndef COMMATERIAL_H
#define COMMATERIAL_H

#include "Component.h"
#include "Globals.h"
#include <vector>
#include "glmath.h"
#include "MathGeoLib\MathGeoLib.h"

struct Texture
{
	std::string name;
	uint id;
};

class GameObject;

class CompMaterial : public Component
{
public:
	CompMaterial(GameObject* parent, COMP_TYPE type);
	~CompMaterial();

	// Set
	void SetID(uint id);

	// Read
	uint const GetTextureId();

private:
	Texture* texture;
};

#endif // !COMMATERIAL_H