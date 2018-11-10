#include "Globals.h"
#include "Application.h"
#include "ModuleCamera3D.h"
#include "CompMesh.h"
#include "ModuleSceneIntro.h"
#include "CompTransform.h"
#include "CompCamera.h"

ModuleCamera3D::ModuleCamera3D(bool start_enabled) : Module(start_enabled)
{
	//CalculateViewMatrix();

	//X = vec3(1.0f, 0.0f, 0.0f);
	//Y = vec3(0.0f, 1.0f, 0.0f);
	//Z = vec3(0.0f, 0.0f, 1.0f);

	//Position = vec3(0.0f, 0.0f, 5.0f);
	//Reference = vec3(0.0f, 0.0f, 0.0f);

	engine_camera = new CompCamera(nullptr, C_CAMERA);
	engine_camera->SetFarPlaneDistance(100.0f);

}

ModuleCamera3D::~ModuleCamera3D()
{
	RELEASE(engine_camera);
}

// -----------------------------------------------------------------
bool ModuleCamera3D::Start()
{
	LOG("Setting up the camera");
	bool ret = true;

	engine_camera->frustum.pos = { 0.0f,1.0f,-5.0f };

	return ret;
}

// -----------------------------------------------------------------
bool ModuleCamera3D::CleanUp()
{
	LOG("Cleaning camera");

	return true;
}

// -----------------------------------------------------------------
update_status ModuleCamera3D::Update(float dt)
{
	// Implement a debug camera with keys and mouse
	// Now we can make this movememnt frame rate independant!

	if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
		speed = 8.0f * dt;

	// Scroll
	if (App->input->GetMouseZ() == 1)
	{
		float3 newPos = engine_camera->frustum.front  * (wheelSpeed * dt);
		engine_camera->frustum.Translate(newPos);
	}
	else if (App->input->GetMouseZ() == -1) {
		float3 newPos = engine_camera->frustum.front  * -(wheelSpeed * dt);
		engine_camera->frustum.Translate(newPos);
	}

	// Pan camera
	if (App->input->GetMouseButton(SDL_BUTTON_MIDDLE) == KEY_REPEAT)
	{
		float3 newPos = float3::zero;

		int dx = -App->input->GetMouseXMotion();
		int dy = App->input->GetMouseYMotion();
		float sensibility = 2.0f * dt;
		newPos += engine_camera->frustum.WorldRight() * dx * sensibility;
		newPos += engine_camera->frustum.up * dy *  sensibility;
		
		engine_camera->frustum.Translate(newPos);
	}
	// Look to object
	if (App->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN)
	{
		CentrateObjectView();
	}

	// Rotate camera with static position
	if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT &&
		App->input->GetKey(SDL_SCANCODE_LALT) == KEY_IDLE)
	{
		int dx = -App->input->GetMouseXMotion();
		int dy = -App->input->GetMouseYMotion();

		float sensibility = 2.0f * dt;
		if (dx != 0 || dy != 0)
		{
			float cameraRotationSpeed = speed * sensibility;
			RotateAroundReference(engine_camera->frustum.pos, dx * cameraRotationSpeed, dy * cameraRotationSpeed);
		}
	}

	// FPS-like movement
	if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_DOWN)
	{
		LOG("FPS-like camera enabled.");
	}
	if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_UP)
	{
		LOG("FPS-like camera disabled.");
	}
	if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT)
	{
		float3 newPos = float3::zero;
		if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) newPos += engine_camera->frustum.front * speed* dt;
		if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) newPos -= engine_camera->frustum.front * speed* dt;

		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) newPos -= engine_camera->frustum.WorldRight() * speed* dt;
		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) newPos += engine_camera->frustum.WorldRight() * speed* dt;

		engine_camera->frustum.Translate(newPos);
	}

	// Camera speed
	if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_DOWN)
		speed *= 2;
	else if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_UP)
		speed = speed / 2;

	return UPDATE_CONTINUE;
}

// -----------------------------------------------------------------
void ModuleCamera3D::RotateAroundReference(const math::float3& reference, float arroundX, float arroundY) const {
	math::Quat rotX = math::Quat::RotateAxisAngle({ 0.0f,1.0f,0.0f }, arroundX * DEGTORAD);
	math::Quat rotY = math::Quat::RotateAxisAngle(engine_camera->frustum.WorldRight(), arroundY * DEGTORAD);
	math::Quat rot = rotX * rotY;

	engine_camera->frustum.up = rot * engine_camera->frustum.up;
	engine_camera->frustum.front = rot * engine_camera->frustum.front;

	float distance = (engine_camera->frustum.pos - reference).Length();
	engine_camera->frustum.pos = reference + (-engine_camera->frustum.front * distance);
}

// -----------------------------------------------------------------
void ModuleCamera3D::LookAt(const float3 &Spot)
{
	math::float3 Z = -(engine_camera->frustum.pos - Spot).Normalized();
	math::float3 X = math::Cross(math::float3(0.0f, 1.0f, 0.0f), Z).Normalized();
	math::float3 Y = math::Cross(Z, X);

	engine_camera->frustum.front = Z;
	engine_camera->frustum.up = Y;
}


void ModuleCamera3D::CentrateObjectView()
{
	if (App->scene_intro->selected_gameObject != nullptr)
	{
		CompMesh* mesh = App->scene_intro->selected_gameObject->GetCompMesh();
		CompTransform* transform = App->scene_intro->selected_gameObject->GetCompTransform();
		if (mesh != nullptr && transform != nullptr)
		{
			math::AABB box(float3(0, 0, 0), float3(0, 0, 0));
			box.Enclose((float3*)mesh->GetVertices(), mesh->GetNumVertices());

			float3 Reference = float3::zero;

			Reference.x = box.CenterPoint().x* transform->GetScale().x + transform->GetPosition().x;
			// Divide Y reference because the console covers the geometry. This way we can see the GameObject.
			Reference.y = (box.CenterPoint().y* transform->GetScale().y) / 2 + transform->GetPosition().y;
			Reference.z = box.CenterPoint().z* transform->GetScale().z + transform->GetPosition().z;

			float3 Position = float3::zero;
			Position.x = (box.maxPoint.x * transform->GetScale().x)*2 + transform->GetPosition().x;
			Position.y = (box.maxPoint.y * transform->GetScale().y)*2 + transform->GetPosition().y;
			Position.z = (box.maxPoint.z * transform->GetScale().z)*2 + transform->GetPosition().z;

			engine_camera->frustum.Translate(Position);
			App->camera->LookAt(Reference);
		}
	}
}

float* ModuleCamera3D::GetViewMatrix() const {
	return (float*)&engine_camera->GetViewMatrix();
}

float4x4 ModuleCamera3D::GetProjectionMatrix() const {
	return engine_camera->GetProjectionMatrix();
}

float3 ModuleCamera3D::GetCameraPos()const {
	return engine_camera->frustum.pos;
}