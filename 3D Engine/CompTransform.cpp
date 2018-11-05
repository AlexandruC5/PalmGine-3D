#include "CompTransform.h"
#include "Component.h"
#include "GameObject.h"

CompTransform::CompTransform(GameObject * parent, COMP_TYPE type) : Component(parent, type)
{
}

CompTransform::~CompTransform()
{
}

void CompTransform::Update(float dt)
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

void CompTransform::SetPosition(math::float3 pos)
{
	position = pos;

	transform_matrix.SetTranslatePart(position);
	if (parent->GetParent() != nullptr)
	{
		CompTransform* tmp_transform = ((CompTransform*)parent->GetParent()->FindComponent(COMP_TYPE::C_TRANSFORM));
		if (tmp_transform != nullptr)
			global_transform = transform_matrix*tmp_transform->GetTransformationMatrix();
	}	
}

void CompTransform::SetRotation(math::float3 rot)
{
	// TODO need fix
	quaternion_rotation = math::Quat::FromEulerXYX(rot.x*DEGTORAD, rot.y*DEGTORAD, rot.z*DEGTORAD);
	transform_matrix = math::float4x4::FromQuat(quaternion_rotation);
	transform_matrix.SetTranslatePart(position);
	rotation = rot;

	if (parent->GetParent() != nullptr)
	{
		CompTransform* tmp_transform = ((CompTransform*)parent->GetParent()->FindComponent(COMP_TYPE::C_TRANSFORM));
		if (tmp_transform != nullptr)
			global_transform = transform_matrix*tmp_transform->GetTransformationMatrix();
	}
}

void CompTransform::SetScale(math::float3 sca)
{
	// TODO scale
	/*scale = sca;
	transform_matrix.Scale(scale);
	global_transform = transform_matrix*inhe_transform;*/
}

math::float3 CompTransform::GetPosition()const
{
	return position;
}

math::float3 CompTransform::GetRotation() const
{
	return rotation;
}

math::float3 CompTransform::GetScale() const
{
	return scale;
}

math::float4x4 CompTransform::GetTransformationMatrix() const {
	return global_transform;
}