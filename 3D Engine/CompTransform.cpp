#include "CompTransform.h"
#include "CompCamera.h"
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

void CompTransform::UpdateTransformationMatrix()
{
	transform_matrix = float4x4::FromTRS(position, quaternion_rotation, scale);
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
		global_transform = transform_matrix*tmp_transform->GetTransformationMatrix();
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
	if (parent->GetCompCamera() != nullptr) {
		parent->GetCompCamera()->frustum.pos = position;
	}

	if (parent->GetNumChilds() > 0) {
		for (int i = 0; i < parent->GetNumChilds(); ++i) {
			parent->childs[i]->GetCompTransform()->RecalculateGlobalMatrix();
		}
	}
}

void CompTransform::SetRotation(math::float3 rot)
{
	quaternion_rotation = math::Quat::FromEulerXYZ(rot.x*DEGTORAD, rot.y*DEGTORAD, rot.z*DEGTORAD);
	if (parent->GetCompCamera() == nullptr) {
		transform_matrix = float4x4::FromTRS(position, quaternion_rotation, scale);
		rotation = rot;
	}
	else {
		transform_matrix = math::float4x4::FromQuat(quaternion_rotation);
		rotation = rot;
	}

	if (parent->GetParent() != nullptr)
	{
		CompTransform* tmp_transform = ((CompTransform*)parent->GetParent()->FindComponent(COMP_TYPE::C_TRANSFORM));
		if (tmp_transform != nullptr)
			global_transform = transform_matrix*tmp_transform->GetTransformationMatrix();
	}
	if (parent->GetCompCamera() != nullptr) {
		parent->GetCompCamera()->frustum.front = global_transform.WorldZ();
		parent->GetCompCamera()->frustum.up = global_transform.WorldY();
	}

	if (parent->GetNumChilds() > 0) {
		for (int i = 0; i < parent->GetNumChilds(); ++i) {
			parent->childs[i]->GetCompTransform()->RecalculateGlobalMatrix();
		}
	}
}

void CompTransform::SetRotationWithQuat(Quat rot) 
{
	quaternion_rotation = rot;
	if (parent->GetCompCamera() == nullptr) {
		transform_matrix = float4x4::FromTRS(position, quaternion_rotation, scale);
	}
	else {
		transform_matrix = math::float4x4::FromQuat(quaternion_rotation);
		transform_matrix.SetTranslatePart(position);
	}
	rotation = rot.ToEulerXYX();

	if (parent->GetParent() != nullptr)
	{
		CompTransform* tmp_transform = ((CompTransform*)parent->GetParent()->FindComponent(COMP_TYPE::C_TRANSFORM));
		if (tmp_transform != nullptr)
			global_transform = transform_matrix * tmp_transform->GetTransformationMatrix();
	}
	if (parent->GetCompCamera() != nullptr) {
		parent->GetCompCamera()->frustum.front = global_transform.WorldZ();
		parent->GetCompCamera()->frustum.up = global_transform.WorldY();
	}

	if (parent->GetNumChilds() > 0) {
		for (int i = 0; i < parent->GetNumChilds(); ++i) {
			parent->childs[i]->GetCompTransform()->RecalculateGlobalMatrix();
		}
	}
}

void CompTransform::SetScale(math::float3 sca)
{
	scale = sca;
	transform_matrix = math::float4x4::FromTRS(position, quaternion_rotation, scale);
	if (parent->GetParent() != nullptr)
	{
		CompTransform* tmp_transform = ((CompTransform*)parent->GetParent()->FindComponent(COMP_TYPE::C_TRANSFORM));
		if (tmp_transform != nullptr)
			global_transform = transform_matrix*tmp_transform->GetTransformationMatrix();
	}
	else
		global_transform = transform_matrix;
}

void CompTransform::RecalculateGlobalMatrix() {
	transform_matrix = math::float4x4::FromTRS(position, quaternion_rotation, scale);
	if (parent->GetParent() != nullptr)
	{
		CompTransform* tmp_transform = ((CompTransform*)parent->GetParent()->FindComponent(COMP_TYPE::C_TRANSFORM));
		if (tmp_transform != nullptr)
			global_transform = transform_matrix*tmp_transform->GetTransformationMatrix();
	}

	if (parent->GetNumChilds() > 0) {
		for (int i = 0; i < parent->GetNumChilds(); ++i) {
			parent->childs[i]->GetCompTransform()->RecalculateGlobalMatrix();
		}
	}

	if (parent->GetNumChilds() > 0) {
		for (int i = 0; i < parent->GetNumChilds(); ++i) {
			parent->childs[i]->GetCompTransform()->RecalculateGlobalMatrix();
		}
	}
}

math::float3 CompTransform::GetPosition()const
{
	return position;
}

math::float3 CompTransform::GetRotation() const
{
	return rotation;
}

math::Quat CompTransform::GetRotationQuat() const
{
	return quaternion_rotation;
}

math::float3 CompTransform::GetScale() const
{
	return scale;
}

math::float4x4 CompTransform::GetLocalTransform() const
{
	return transform_matrix;
}

math::float4x4 CompTransform::GetTransformationMatrix() const 
{
	if (parent->GetParent() != nullptr)
	{
		CompTransform* trans = parent->GetParent()->GetCompTransform();
		return trans->GetTransformationMatrix()* transform_matrix;
	}
	return transform_matrix;
}

uint CompTransform::GetSize() 
{
	// GET SIZE FOR SERIALIZATION
	uint size = 0;

	// COMPONENT TYPE
	size += sizeof(int);
	// IS ACTIVE
	size += sizeof(int);
	// POSITION
	size += sizeof(float3);
	// ROTATION
	size += sizeof(float3);
	// SCALE
	size += sizeof(float3);

	return size;
}

void CompTransform::WriteComponentData(char ** cursor)
{
	uint bytes = 0;

	// COMPONENT TYPE
	bytes = sizeof(int);
	memcpy(cursor[0], &type, bytes);
	cursor[0] += bytes;
	// IS ACTIVE
	bytes = sizeof(int);
	int tmp_active = (int)active;
	memcpy(cursor[0], &tmp_active, bytes);
	cursor[0] += bytes;
	// POSITION
	bytes = sizeof(float3);
	memcpy(cursor[0], &position, bytes);
	cursor[0] += bytes;
	// ROTATION
	bytes = sizeof(float3);
	memcpy(cursor[0], &rotation, bytes);
	cursor[0] += bytes;
	// SCALE
	bytes = sizeof(float3);
	memcpy(cursor[0], &scale, bytes);
	cursor[0] += bytes;
}
