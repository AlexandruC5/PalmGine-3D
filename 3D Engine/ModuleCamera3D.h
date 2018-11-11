#ifndef MODULECAMERA3D_H
#define MODULECAMERA3D_H

#include "Module.h"
#include "Globals.h"
#include "MathGeoLib/MathGeoLib.h"

class CompCamera;

class ModuleCamera3D : public Module
{
public:
	ModuleCamera3D(bool start_enabled = true);
	~ModuleCamera3D();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	void RotateAroundReference(const math::float3& reference, float ArroundX, float ArroundY) const;
	void LookAt(const float3 &Spot);
	void CentrateObjectView();
	float4x4 GetViewMatrix() const;
	float4x4 GetProjectionMatrix() const;

	float3 GetCameraPos()const;

	void DebugDrawPicker();

public:

	float speed = 3.0f;
	float wheelSpeed = 3.0f;

	CompCamera* engine_camera = nullptr;


private:
	LineSegment picker;

};

#endif // !MODULECAMERA3D_H