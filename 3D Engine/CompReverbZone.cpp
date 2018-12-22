#include "CompReverbZone.h"
#include "Application.h"
#include "CompTransform.h"
#include "CompAudioSource.h"
#include "ModuleSceneIntro.h"
#include "ModuleAudio.h"

#define MAX(a,b) ((a)>(b)) ? (a) : (b)

CompReverbZone::CompReverbZone(GameObject * parent, COMP_TYPE type) : Component(parent, type)
{
	
}

CompReverbZone::~CompReverbZone()
{
	
}

void CompReverbZone::Update(float dt)
{

	sphere.pos = parent->GetCompTransform()->GetPosition();
	cube.Translate(parent->GetCompTransform()->GetPosition());

	if (use_sphere == true) {
		DebugDrawSphere();
		for (int i = 0; i < App->scene_intro->root_gameObjects->GetNumChilds(); ++i) {
			CompAudioSource* audio_source = App->scene_intro->root_gameObjects->childs[i]->GetCompAudioSource();
			if (audio_source != nullptr) {
				if (sphere.Intersects(App->scene_intro->root_gameObjects->childs[i]->GetAABB()) == true) {
					audio_source->source->ApplyEnvReverb(12, "tunnel");
				}
				else {
					audio_source->source->ApplyEnvReverb(0, "tunnel");
				}
			}
		}
	}
	else {
		DebugDrawCube();
		for (int i = 0; i < App->scene_intro->root_gameObjects->GetNumChilds(); ++i) {
			CompAudioSource* audio_source = App->scene_intro->root_gameObjects->childs[i]->GetCompAudioSource();
			if (audio_source != nullptr) {
				if (cube.Intersects(App->scene_intro->root_gameObjects->childs[i]->GetAABB()) == true) {
					audio_source->source->ApplyEnvReverb(12, "tunnel");
				}
				else {
					audio_source->source->ApplyEnvReverb(0, "tunnel");
				}
			}
		}
	}
}

//Getters
float CompReverbZone::GetRadius() const
{
	return radius;
}
bool CompReverbZone::UsingSphere() const
{
	return use_sphere;
}
bool CompReverbZone::UsingCube() const
{
	return use_cube;
}

//Setters
void CompReverbZone::SetRadius(float rad) {
	radius = rad;
	sphere.r = rad;
	cube.r = math::float3(rad,rad,rad);
}
void CompReverbZone::ShouldUseSphere(bool should_use)
{
	use_sphere = should_use;
	use_cube = !should_use;
}
void CompReverbZone::ShouldUseCube(bool should_use)
{
	use_sphere = !should_use;
	use_cube = should_use;
}

//DebugDraw
void CompReverbZone::DebugDrawSphere()
{
	glColor3f(0, 128, 128);

	glPushMatrix();
	glMultMatrixf((GLfloat*) parent->GetCompTransform()->GetTransformationMatrix().ptr());

	int stacks = MAX(5, (int)sphere.r * 10);
	int slices = MAX(5, (int)sphere.r * 10);

	int i, j;
	for (j = 0; j < stacks; j++) {
		double latitude1 = (math::pi / stacks) * j - math::pi / 2;
		double latitude2 = (math::pi / stacks) * (j + 1) - math::pi / 2;
		double sinLat1 = sin(latitude1);
		double cosLat1 = cos(latitude1);
		double sinLat2 = sin(latitude2);
		double cosLat2 = cos(latitude2);
		glBegin(GL_QUAD_STRIP);
		for (i = 0; i <= slices; i++) {
			double longitude = (2 * math::pi / slices) * i;
			double sinLong = sin(longitude);
			double cosLong = cos(longitude);
			double x1 = cosLong * cosLat1;
			double y1 = sinLong * cosLat1;
			double z1 = sinLat1;
			double x2 = cosLong * cosLat2;
			double y2 = sinLong * cosLat2;
			double z2 = sinLat2;
			glNormal3d(x2, y2, z2);
			glVertex3d(sphere.pos.x + sphere.r*x2, sphere.pos.y + sphere.r*y2, sphere.pos.z + sphere.r*z2);
			glNormal3d(x1, y1, z1);
			glVertex3d(sphere.pos.x + sphere.r*x1, sphere.pos.y + sphere.r*y1, sphere.pos.z + sphere.r*z1);
		}
		glEnd();
	}
	glPopMatrix();
}
void CompReverbZone::DebugDrawCube()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glBegin(GL_QUADS);
	glLineWidth(5.0f);
	glColor4f(1.f, 1.f, 0.f, 1.f);

	float3 vertices[8];
	cube.GetCornerPoints(vertices);

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

	glColor4f(1.f, 1.f, 1.f, 1.f);
	glEnd();
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}