#include "CompTransform.h"

CompTransform::CompTransform(GameObject * parent, COMP_TYPE type) : Component(parent, type)
{
}

CompTransform::~CompTransform()
{
}

void CompTransform::SetRotation(math::float3 rot)
{
	rotation = rot;
}

math::float3 const CompTransform::GetPosition()
{
	return position;
}

math::float3 const CompTransform::GetRotation()
{
	return rotation;
}

math::float3 const CompTransform::GetScale()
{
	return scale;
}
