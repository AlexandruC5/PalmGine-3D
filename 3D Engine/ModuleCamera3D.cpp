#include "Globals.h"
#include "Application.h"
#include "ModuleCamera3D.h"

ModuleCamera3D::ModuleCamera3D(bool start_enabled) : Module(start_enabled)
{
	CalculateViewMatrix();

	X = vec3(1.0f, 0.0f, 0.0f);
	Y = vec3(0.0f, 1.0f, 0.0f);
	Z = vec3(0.0f, 0.0f, 1.0f);

	Position = vec3(0.0f, 0.0f, 5.0f);
	Reference = vec3(0.0f, 0.0f, 0.0f);
}

ModuleCamera3D::~ModuleCamera3D()
{}

// -----------------------------------------------------------------
bool ModuleCamera3D::Start()
{
	LOG("Setting up the camera");
	bool ret = true;

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
	vec3 newPos(0, 0, 0);
	Position += newPos;
	Reference += newPos;

	// Rotate object
	if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT)
	{
		int dx = -App->input->GetMouseXMotion();
		int dy = -App->input->GetMouseYMotion();

		float Sensitivity = 0.25f;

		Position -= Reference;

		if (dx != 0)
		{
			float DeltaX = (float)dx * Sensitivity;

			X = rotate(X, DeltaX, vec3(0.0f, 1.0f, 0.0f));
			Y = rotate(Y, DeltaX, vec3(0.0f, 1.0f, 0.0f));
			Z = rotate(Z, DeltaX, vec3(0.0f, 1.0f, 0.0f));
		}

		if (dy != 0)
		{
			float DeltaY = (float)dy * Sensitivity;

			Y = rotate(Y, DeltaY, X);
			Z = rotate(Z, DeltaY, X);

			if (Y.y < 0.0f)
			{
				Z = vec3(0.0f, Z.y > 0.0f ? 1.0f : -1.0f, 0.0f);
				Y = cross(Z, X);
			}
		}

		Position = Reference + Z * length(Position);
	}

	// Scroll
	if (App->input->GetMouseZ() == 1)
	{
		newPos -= Z * wheelSpeed * 5 * dt;
		Position += newPos;
		Reference += newPos;

		AABB box = App->fbx->GetAABB();
		Reference.x = box.CenterPoint().x;
		Reference.y = box.CenterPoint().y;
		Reference.z = box.CenterPoint().z;
	}
	else if (App->input->GetMouseZ() == -1)
	{
		newPos += Z * wheelSpeed * 5 * dt;
		Position += newPos;
		Reference += newPos;

		AABB box = App->fbx->GetAABB();
		Reference.x = box.CenterPoint().x;
		Reference.y = box.CenterPoint().y;
		Reference.z = box.CenterPoint().z;
	}

	// Pan camera
	if (App->input->GetMouseButton(SDL_BUTTON_MIDDLE) == KEY_REPEAT)
	{
		if (App->input->GetMouseXMotion() > 0)
		{
			newPos -= X * (App->input->GetMouseXMotion() * dt) * speed;
		}
		if (App->input->GetMouseXMotion() < 0)
		{
			newPos -= X * (App->input->GetMouseXMotion() * dt) * speed;
		}
		if (App->input->GetMouseYMotion() > 0)
		{
			newPos += Y * (App->input->GetMouseYMotion() * dt) * speed;
		}
		if (App->input->GetMouseYMotion() < 0)
		{
			newPos += Y * (App->input->GetMouseYMotion() * dt) * speed;
		}

		Position += newPos;
		Reference += newPos;
	}
	// Look to object
	if (App->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN)
	{
		App->fbx->CentrateObjectView();
	}

	// Rotate camera with static position
	if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT &&
		App->input->GetKey(SDL_SCANCODE_LALT) == KEY_IDLE)
	{
		int dx = -App->input->GetMouseXMotion();
		int dy = -App->input->GetMouseYMotion();

		float sensibility = 0.15f;
		if (dx != 0)
		{
			float DeltaX = (float)dx * sensibility;
			X = rotate(X, DeltaX, vec3(0.0f, 1.0f, 0.0f));
			Y = rotate(Y, DeltaX, vec3(0.0f, 1.0f, 0.0f));
			Z = rotate(Z, DeltaX, vec3(0.0f, 1.0f, 0.0f));
		}
		if (dy != 0)
		{
			float DeltaY = (float)dy * sensibility;
			Y = rotate(Y, DeltaY, X);
			Z = rotate(Z, DeltaY, X);
			if (Y.y < 0.0f)
			{
				Z = vec3(0.0f, Z.y > 0.0f ? 1.0f : -1.0f, 0.0f);
				Y = cross(Z, X);
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
		if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) newPos -= Z * speed* dt;
		if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) newPos += Z * speed* dt;

		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) newPos -= X * speed* dt;
		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) newPos += X * speed* dt;

		Position += newPos;
		Reference += newPos;
	}

	// Camera speed
	if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_DOWN)
		speed *= 2;
	else if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_UP)
		speed = speed / 2;

	// Recalculate matrix -------------
	CalculateViewMatrix();

	return UPDATE_CONTINUE;
}

// -----------------------------------------------------------------
void ModuleCamera3D::Look(const vec3 &Position, const vec3 &Reference, bool RotateAroundReference)
{
	this->Position = Position;
	this->Reference = Reference;

	Z = normalize(Position - Reference);
	X = normalize(cross(vec3(0.0f, 1.0f, 0.0f), Z));
	Y = cross(Z, X);

	if (!RotateAroundReference)
	{
		this->Reference = this->Position;
		this->Position += Z * 0.05f;
	}

	CalculateViewMatrix();
}

// -----------------------------------------------------------------
void ModuleCamera3D::LookAt(const vec3 &Spot)
{
	Reference = Spot;

	Z = normalize(Position - Reference);
	X = normalize(cross(vec3(0.0f, 1.0f, 0.0f), Z));
	Y = cross(Z, X);

	CalculateViewMatrix();
}


// -----------------------------------------------------------------
void ModuleCamera3D::Move(const vec3 &Movement)
{
	Position += Movement;
	Reference += Movement;

	CalculateViewMatrix();
}

// -----------------------------------------------------------------
float* ModuleCamera3D::GetViewMatrix()
{
	return &ViewMatrix;
}

// -----------------------------------------------------------------
void ModuleCamera3D::CalculateViewMatrix()
{
	ViewMatrix = mat4x4(X.x, Y.x, Z.x, 0.0f, X.y, Y.y, Z.y, 0.0f, X.z, Y.z, Z.z, 0.0f, -dot(X, Position), -dot(Y, Position), -dot(Z, Position), 1.0f);
	ViewMatrixInverse = inverse(ViewMatrix);
}