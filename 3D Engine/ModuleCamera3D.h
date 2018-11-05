#ifndef MODULECAMERA3D_H
#define MODULECAMERA3D_H

#include "Module.h"
#include "Globals.h"
#include "glmath.h"

class ModuleCamera3D : public Module
{
public:
	ModuleCamera3D(bool start_enabled = true);
	~ModuleCamera3D();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	void Look(const vec3 &Position, const vec3 &Reference, bool RotateAroundReference = false);
	void LookAt(const vec3 &Spot);
	void Move(const vec3 &Movement);
	void CentrateObjectView();
	float* GetViewMatrix();

private:

	void CalculateViewMatrix();

public:
	
	vec3 X, Y, Z, Position, Reference;

	float speed = 3.0f;
	float wheelSpeed = 3.0f;

private:

	mat4x4 ViewMatrix, ViewMatrixInverse;
};

#endif // !MODULECAMERA3D_H