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