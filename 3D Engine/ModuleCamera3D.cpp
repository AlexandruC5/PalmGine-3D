#include "Globals.h"
#include "Application.h"
#include "ModuleCamera3D.h"
#include "CompMesh.h"
#include "ModuleSceneIntro.h"
#include "CompTransform.h"
#include "CompCamera.h"
#include "ImGuizmo-master/ImGuizmo.h"

ModuleCamera3D::ModuleCamera3D(bool start_enabled) : Module(start_enabled)
{
	engine_camera = new CompCamera(nullptr, C_CAMERA);
	engine_camera->SetFarPlaneDistance(100.0f);
	speed = 6.0f;
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
	if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_REPEAT &&
		App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT)
	{
		int dx = -App->input->GetMouseXMotion();
		int dy = -App->input->GetMouseYMotion();



		float sensibility = 4.0f * dt;
		if (dx != 0 || dy != 0)
		{
			float cameraRotationSpeed = speed * sensibility;
			if (App->scene_intro->selected_gameObject !=nullptr) {
				RotateAroundReference(App->scene_intro->selected_gameObject->GetCompTransform()->GetPosition(), dx * cameraRotationSpeed, dy * cameraRotationSpeed);
			}
			else {
				RotateAroundReference(float3::zero, dx * cameraRotationSpeed, dy * cameraRotationSpeed);
			}
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
	if (ImGui::IsMouseHoveringAnyWindow() == false && ImGuizmo::IsOver() == false) {
		if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN && App->input->GetKey(SDL_SCANCODE_LALT) == KEY_IDLE) {
			float window_width = (float)App->window->screen_surface->w;
			float window_height = (float)App->window->screen_surface->h;
			int mouseX = App->input->GetMouseX();
			int mouseY = App->input->GetMouseY();

			float mouseX_normalized = -(1.0f - (float(mouseX) * 2.0f) / window_width);
			float mouseY_normalized = 1.0f - (float(mouseY) * 2.0f) / window_height;

			picker = engine_camera->frustum.UnProjectLineSegment(mouseX_normalized, mouseY_normalized);

			App->scene_intro->PickGO(picker);
		}
	}
	//DebugDrawPicker();

	// Camera speed
	if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_DOWN)
		speed *= 2;
	else if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_UP)
		speed = speed / 2;

	return UPDATE_CONTINUE;
}

// -----------------------------------------------------------------
void ModuleCamera3D::RotateAroundReference(const math::float3& reference, float arroundX, float arroundY) const 
{
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
			math::AABB box = App->scene_intro->selected_gameObject->GetAABB();
			float3 Reference = float3::zero;

			Reference = box.CenterPoint();
			float3 Position = float3::zero;
			Position = ((box.CenterPoint() + box.maxPoint) - box.CenterPoint())*2;

			engine_camera->frustum.pos = Position;
			App->camera->LookAt(Reference);
		}
	}
}

float4x4 ModuleCamera3D::GetViewMatrix() const 
{
	return engine_camera->GetViewMatrix();
}

float4x4 ModuleCamera3D::GetProjectionMatrix() const 
{
	return engine_camera->GetProjectionMatrix();
}

float3 ModuleCamera3D::GetCameraPos()const 
{
	return engine_camera->frustum.pos;
}

void ModuleCamera3D::DebugDrawPicker() {
	glBegin(GL_LINES);
	glLineWidth(5.0f);
	glColor4f(1.f, 0.f, 0.f, 1.f);

	picker.a.x;
	glVertex3f(picker.a.x, picker.a.y, picker.a.z);
	glVertex3f(picker.b.x, picker.b.y, picker.b.z);

	glColor4f(1.f, 1.f, 1.f, 1.f);
	glEnd();
}