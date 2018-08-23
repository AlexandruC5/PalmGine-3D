#pragma once
#include "Component.h"
#include <vector>
#include "MathGeoLib\MathGeoLib.h"

struct Axis
{
	float3 x = { 1, 0, 0 };
	float3 y = { 0, 1, 0 };
	float3 z = { 0, 0, 1 };
};

class GameObject;

class CompTransform : public Component
{
public:
	CompTransform(GameObject* parent, COMP_TYPE type);
	~CompTransform();

	void SetRotation(float3 rot);

private:
	Axis axis;

	// Propierties
	float3 position = { 0, 0, 0 };
	float3 rotation = { 0, 0, 0 };
	float3 scale = { 0, 0, 0 };
};