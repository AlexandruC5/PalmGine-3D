#ifndef COMPMOVEMENT_H
#define COMPMOVEMENT_H

#include "Component.h"
#include "Globals.h"
#include "MathGeoLib\MathGeoLib.h"

class GameObject;

class CompMovement : public Component
{
public:
	CompMovement(GameObject* parent, COMP_TYPE type);
	~CompMovement();
	void Update(float dt);

	//Getters
	math::float3 GetPosA()const;
	math::float3 GetPosB()const;

	//Setters
	void SetPosA(math::float3 posA);
	void SetPosB(math::float3 posB);
	void SetGoingA(bool going);
	void SetGoingB(bool going);

	//Serialization
	uint GetSize() const;
	void WriteComponentData(char ** cursor);

private:
	math::float3 pos_a;
	math::float3 pos_b;

	bool going_a = true;
	bool going_b = false;

};

#endif // !COMPMOVEMENT_H