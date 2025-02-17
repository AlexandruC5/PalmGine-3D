#ifndef COMPTRANSFORM_H
#define COMPTRANSFORM_H

#include "Component.h"
#include <vector>
#include "MathGeoLib/MathGeoLib.h"

struct Axis
{
	math::float3 x = { 1, 0, 0 };
	math::float3 y = { 0, 1, 0 };
	math::float3 z = { 0, 0, 1 };
};

class GameObject;

class CompTransform : public Component
{
public:
	CompTransform(GameObject* parent, COMP_TYPE type);
	~CompTransform();

	void Update(float dt);
	void UpdateTransformationMatrix();

	// Set methods
	void SetTransformation(math::float4x4 new_trans);
	void SetPosition(math::float3 pos);
	void SetRotation(math::float3 rot);
	void SetRotationWithQuat(math::Quat rot);
	void SetScale(math::float3 scale);

	// Get methods
	math::float3 GetPosition() const;
	math::float3 GetRotation() const;
	math::Quat GetRotationQuat() const;
	math::float3 GetScale() const;
	math::float4x4 GetLocalTransform()const;
	math::float4x4 GetTransformationMatrix() const;
	uint GetSize()const;

	//Utility
	void RecalculateGlobalMatrix();

	// Write
	void WriteComponentData(char** cursor);

private:
	Axis axis;
	// Propierties
	math::float3 position = { 0, 0, 0 };
	math::float3 rotation = { 0, 0, 0 };
	math::float3 scale = { 1, 1, 1 };
	// Transform and rotation
	math::float4x4 transform_matrix = math::float4x4::identity;
	math::float3 euler_rotation = { 0,0,0 };
	math::Quat quaternion_rotation = math::Quat::identity;
	// Inherit from parent
	math::float4x4	inhe_transform = math::float4x4::identity;
	// Global = inherit * own_transformation
	float4x4 global_transform = float4x4::identity;
};

#endif // !COMPTRANSFORM_H