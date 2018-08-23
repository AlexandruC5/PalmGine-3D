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
