#ifndef COMPREVERBZONE_H
#define COMPREVERBZONE_H

#include "Component.h"
#include "Globals.h"
#include "MathGeoLib\MathGeoLib.h"

class GameObject;

class CompReverbZone : public Component
{
public:
	CompReverbZone(GameObject* parent, COMP_TYPE type);
	~CompReverbZone();
	void Update(float dt);

	//Getters
	float GetRadius() const;
	bool UsingSphere() const;
	bool UsingCube() const;

	//Setters
	void SetRadius(float rad);
	void ShouldUseSphere(bool should_use);
	void ShouldUseCube(bool should_use);
	void SetSphere(math::Sphere s);
	void SetCube(math::OBB c);

	//DebugDraw
	void DebugDrawSphere();
	void DebugDrawCube();

	//Serialization
	uint GetSize() const;
	void WriteComponentData(char ** cursor);

private:
	math::Sphere sphere;
	math::OBB cube;

	float radius = 10.0f;

	bool use_sphere = true;
	bool use_cube = false;
};

#endif // !COMPREVERBZONE_H