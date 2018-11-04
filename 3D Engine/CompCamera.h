#ifndef COMCAMERA_H
#define COMCAMERA_H

#include "Component.h"
#include "Globals.h"
#include <vector>
#include "MathGeoLib/MathGeoLib.h"

class GameObject;

class CompCamera : public Component
{
public:
	CompCamera(GameObject* parent, COMP_TYPE type);
	~CompCamera();
	void Update(float dt);

	void DebugDraw();

	//Getters
	float GetNearPlaneDistance() const;
	float GetFarPlaneDistance() const;
	float GetFOV() const;
	float GetApectRatio() const;

	//Setters
	void SetNearPlaneDistance(float distance);
	void SetFarPlaneDistance(float distance);
	void SetFOV(float fov);
	void SetAspectRatio(float aspect_ratio);

	void Look(const float3& position);

public:
	Frustum frustum;
	bool frustum_culling = false;
	bool active = true;
};

#endif // !COMCAMERA_H