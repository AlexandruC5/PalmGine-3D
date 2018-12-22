#include "CompMovement.h"
#include "Application.h"
#include "CompTransform.h"

CompMovement::CompMovement(GameObject * parent, COMP_TYPE type) : Component(parent, type)
{
	pos_a = math::float3(5, parent->GetCompTransform()->GetPosition().y, parent->GetCompTransform()->GetPosition().z);
	pos_b = math::float3(-5, parent->GetCompTransform()->GetPosition().y, parent->GetCompTransform()->GetPosition().z);
}

CompMovement::~CompMovement()
{
	
}

void CompMovement::Update(float dt)
{
	math::float3 curr_pos = parent->GetCompTransform()->GetPosition();

	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;

	if (going_a == true) {
		if (curr_pos.x < pos_a.x) {
			x += 1 * dt;
		}
		else {
			going_a = false;
			going_b = true;
		}
	}
	else {
		if (curr_pos.x > pos_b.x) {
			x -= 1 * dt;
		}
		else {
			going_a = true;
			going_b = false;
		}
	}

	math::float3 move = math::float3(x,y,z);
	move += curr_pos;
	parent->GetCompTransform()->SetPosition(move);

}

//Getters
math::float3 CompMovement::GetPosA()const
{
	return pos_a;
}
math::float3 CompMovement::GetPosB()const
{
	return pos_b;
}

//Setters
void CompMovement::SetPosA(math::float3 posA)
{
	pos_a = posA;
}
void CompMovement::SetPosB(math::float3 posB)
{
	pos_b = posB;
}

void CompMovement::SetGoingA(bool going)
{
	going_a = going;
}

void CompMovement::SetGoingB(bool going)
{
	going_b = going;
}

uint CompMovement::GetSize() const
{
	// GET SIZE FOR SERIALIZATION
	uint size = 0;

	// COMPONENT TYPE
	size += sizeof(int);
	// IS ACTIVE
	size += sizeof(int);
	// POS A
	size += sizeof(float3);
	// POS B
	size += sizeof(float3);
	// GOING A
	size += sizeof(int);
	// GOING B
	size += sizeof(int);

	return size;
}

void CompMovement::WriteComponentData(char ** cursor)
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
	// POS A
	bytes = sizeof(float3);
	float3 tmp_pos_a = pos_a;
	memcpy(cursor[0], &tmp_pos_a, bytes);
	cursor[0] += bytes;
	// POS B
	bytes = sizeof(float3);
	float3 tmp_pos_b = pos_b;
	memcpy(cursor[0], &tmp_pos_b, bytes);
	cursor[0] += bytes;
	// GOING A
	bytes = sizeof(int);
	int tmp_going_a = (int)going_a;
	memcpy(cursor[0], &tmp_going_a, bytes);
	cursor[0] += bytes;
	// GOING B
	bytes = sizeof(int);
	int tmp_going_b = (int)going_b;
	memcpy(cursor[0], &tmp_going_b, bytes);
	cursor[0] += bytes;
}
