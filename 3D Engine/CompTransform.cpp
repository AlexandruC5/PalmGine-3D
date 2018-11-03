#include "CompTransform.h"
#include "Component.h"
#include "GameObject.h"

CompTransform::CompTransform(GameObject * parent, COMP_TYPE type) : Component(parent, type)
{
}

CompTransform::~CompTransform()
{
}

void CompTransform::SetTransformation(math::float4x4 new_trans)
{
	transform_matrix = new_trans;

	transform_matrix.Decompose(position, quaternion_rotation, scale);
	euler_rotation = quaternion_rotation.ToEulerXYZ();
	// Inherate from parent if it have
	if (parent->GetParent()->IsRootGo() == false)
	{
		CompTransform* tmp_transform = ((CompTransform*)parent->GetParent()->FindComponent(COMP_TYPE::C_TRANSFORM));
		inhe_transform = transform_matrix.Transposed() * tmp_transform->inhe_transform;
	}
	else
	{
		inhe_transform = transform_matrix.Transposed();
	}
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
