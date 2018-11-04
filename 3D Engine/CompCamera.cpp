#include "CompCamera.h"

#include "Devil/include/il.h"
#include "Devil/include/ilut.h"

CompCamera::CompCamera(GameObject * parent, COMP_TYPE type) : Component(parent, type)
{
	frustum.type = FrustumType::PerspectiveFrustum;
	
	frustum.pos = float3::zero;
	frustum.front = float3::unitZ;
	frustum.up = float3::unitY;

	frustum.nearPlaneDistance = 0.1f;
	frustum.farPlaneDistance = 1000.0f;
	frustum.verticalFov = DEGTORAD * 60.0f;
	SetAspectRatio(1.3f);
}

CompCamera::~CompCamera()
{
}

void CompCamera::Update(float dt) {
	DebugDraw();
}

void CompCamera::DebugDraw() {
	float3 vertices[8];
	frustum.GetCornerPoints(vertices);

	glBegin(GL_QUADS);

	glVertex3fv((GLfloat*)&vertices[1]);
	glVertex3fv((GLfloat*)&vertices[5]);
	glVertex3fv((GLfloat*)&vertices[7]);
	glVertex3fv((GLfloat*)&vertices[3]);

	glVertex3fv((GLfloat*)&vertices[4]);
	glVertex3fv((GLfloat*)&vertices[0]);
	glVertex3fv((GLfloat*)&vertices[2]);
	glVertex3fv((GLfloat*)&vertices[6]);

	glVertex3fv((GLfloat*)&vertices[5]);
	glVertex3fv((GLfloat*)&vertices[4]);
	glVertex3fv((GLfloat*)&vertices[6]);
	glVertex3fv((GLfloat*)&vertices[7]);

	glVertex3fv((GLfloat*)&vertices[0]);
	glVertex3fv((GLfloat*)&vertices[1]);
	glVertex3fv((GLfloat*)&vertices[3]);
	glVertex3fv((GLfloat*)&vertices[2]);

	glVertex3fv((GLfloat*)&vertices[3]);
	glVertex3fv((GLfloat*)&vertices[7]);
	glVertex3fv((GLfloat*)&vertices[6]);
	glVertex3fv((GLfloat*)&vertices[2]);

	glVertex3fv((GLfloat*)&vertices[0]);
	glVertex3fv((GLfloat*)&vertices[4]);
	glVertex3fv((GLfloat*)&vertices[5]);
	glVertex3fv((GLfloat*)&vertices[1]);

	glEnd();

}

float CompCamera::GetNearPlaneDistance() const{
	return frustum.NearPlaneDistance();
}

float CompCamera::GetFarPlaneDistance() const {
	return frustum.FarPlaneDistance();
}

float CompCamera::GetFOV() const {
	return frustum.VerticalFov() * RADTODEG;
}

float CompCamera::GetApectRatio() const {
	return frustum.AspectRatio();
}

void CompCamera::SetNearPlaneDistance(float distance) {
	if (distance > 0.0f && distance < frustum.FarPlaneDistance()) {
		frustum.nearPlaneDistance = distance;
	}
}

void CompCamera::SetFarPlaneDistance(float distance) {
	if (distance > 0.0f && distance > frustum.NearPlaneDistance()) {
		frustum.farPlaneDistance = distance;
	}
}

void CompCamera::SetFOV(float fov) {
	float aspect_ratio = frustum.AspectRatio();
	frustum.verticalFov = fov * DEGTORAD;
	SetAspectRatio(aspect_ratio);
}

void CompCamera::SetAspectRatio(float aspect_ratio) {
	frustum.horizontalFov = 2.f * atanf(tanf(frustum.verticalFov * 0.5f) * aspect_ratio);
}

void CompCamera::Look(const float3& position) {
	float3 dir = position - frustum.pos;

	float3x3 m = float3x3::LookAt(frustum.front, dir.Normalized(), frustum.up, float3::unitY);

	frustum.front = m.MulDir(frustum.front).Normalized();
	frustum.up = m.MulDir(frustum.up).Normalized();
}