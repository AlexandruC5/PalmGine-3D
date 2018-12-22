#include "CompReverbZone.h"
#include "Application.h"
#include "CompTransform.h"
#include "CompAudioSource.h"
#include "ModuleSceneIntro.h"
#include "ModuleAudio.h"

#define MAX(a,b) ((a)>(b)) ? (a) : (b)

CompReverbZone::CompReverbZone(GameObject * parent, COMP_TYPE type) : Component(parent, type)
{}

CompReverbZone::~CompReverbZone()
{}

void CompReverbZone::Update(float dt)
{
	sphere.pos = parent->GetCompTransform()->GetPosition();
	cube.pos = parent->GetCompTransform()->GetPosition();

	if (use_sphere == true) {
		DebugDrawSphere();
		for (int i = 0; i < App->scene_intro->root_gameObjects->GetNumChilds(); ++i) 
		{
			CompAudioSource* audio_source = App->scene_intro->root_gameObjects->childs[i]->GetCompAudioSource();
			if (audio_source != nullptr) 
			{
				if (sphere.Intersects(App->scene_intro->root_gameObjects->childs[i]->GetAABB()) == true) 
				{
					audio_source->source->ApplyEnvReverb(12, "tunnel");
				}
				else 
				{
					audio_source->source->ApplyEnvReverb(0, "tunnel");
				}
			}
		}
	}
	else 
	{
		DebugDrawCube();
		for (int i = 0; i < App->scene_intro->root_gameObjects->GetNumChilds(); ++i) 
		{
			CompAudioSource* audio_source = App->scene_intro->root_gameObjects->childs[i]->GetCompAudioSource();
			if (audio_source != nullptr) 
			{
				if (cube.Intersects(App->scene_intro->root_gameObjects->childs[i]->GetAABB()) == true) 
				{
					audio_source->source->ApplyEnvReverb(12, "tunnel");
				}
				else 
				{
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
void CompReverbZone::SetRadius(float rad) 
{
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

void CompReverbZone::SetSphere(math::Sphere s)
{
	sphere = s;
}

void CompReverbZone::SetCube(math::OBB c)
{
	cube = c;
}

//DebugDraw
void CompReverbZone::DebugDrawSphere()
{
	glLineWidth(3.0f);
	glColor3f(2.0f, 2.0f, 2.0f);

	float radius = sphere.r;
	float3 pos = sphere.pos;
	float degInRad = 360.0f / 12;
	degInRad *= DEGTORAD;
	glBegin(GL_LINE_LOOP);
	for (unsigned int i = 0; i < 12; i++)
		glVertex3f(cos(degInRad * i) * radius + pos.x, pos.y, sin(degInRad * i) * radius + pos.z);
	glEnd();
	glBegin(GL_LINE_LOOP);
	for (unsigned int i = 0; i < 12; i++)
		glVertex3f(cos(degInRad * i) * radius + pos.x, sin(degInRad * i) * radius + pos.y, pos.z);
	glEnd();
	glBegin(GL_LINE_LOOP);
	for (unsigned int i = 0; i < 12; i++)
		glVertex3f(pos.x, sin(degInRad * i) * radius + pos.y, cos(degInRad * i) * radius + pos.z);
	glEnd();

	glLineWidth(1.0f);
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

uint CompReverbZone::GetSize() const
{
	// GET SIZE FOR SERIALIZATION
	uint size = 0;

	// COMPONENT TYPE
	size += sizeof(int);
	// IS ACTIVE
	size += sizeof(int);
	// SHPERE
	size += sizeof(Sphere);
	// CUBE
	size += sizeof(OBB);
	// RADIUS
	size += sizeof(float);
	// USE SPHERE
	size += sizeof(int);
	// USE CUBE
	size += sizeof(int);
	return size;
}

void CompReverbZone::WriteComponentData(char ** cursor)
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
	// SHPERE
	bytes = sizeof(Sphere);
	Sphere tmp_sphere = sphere;
	memcpy(cursor[0], &tmp_sphere, bytes);
	cursor[0] += bytes;
	// CUBE
	bytes = sizeof(OBB);
	OBB tmp_cube = cube;
	memcpy(cursor[0], &tmp_cube, bytes);
	cursor[0] += bytes;
	// RADIUS
	bytes = sizeof(float);
	float tmp_rad = radius;
	memcpy(cursor[0], &tmp_rad, bytes);
	cursor[0] += bytes;
	// USE SPHERE
	bytes = sizeof(int);
	int tmp_usphere = use_sphere;
	memcpy(cursor[0], &tmp_usphere, bytes);
	cursor[0] += bytes;
	// USE CUBE
	bytes = sizeof(int);
	int tmp_ubox = use_cube;
	memcpy(cursor[0], &tmp_ubox, bytes);
	cursor[0] += bytes;
}
