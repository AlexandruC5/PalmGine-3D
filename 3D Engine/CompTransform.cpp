#include "CompTransform.h"

CompTransform::CompTransform(GameObject * parent, COMP_TYPE type) : Component(parent, type)
{
}

CompTransform::~CompTransform()
{
}

void CompTransform::SetRotation(float3 rot)
{
	rotation = rot;
}

float3 const CompTransform::GetPosition()
{
	return position;
}

float3 const CompTransform::GetRotation()
{
	return rotation;
}

float3 const CompTransform::GetScale()
{
	return scale;
}
