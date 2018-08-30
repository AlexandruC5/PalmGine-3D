#pragma once
#include <string>

enum COMP_TYPE
{
	C_UNKNOWN = 0,
	C_TRANSFORM,
	C_MATERIAL,
	C_MESH,
	C_CAMERA
};

class GameObject;

class Component
{
public:
	Component(GameObject* parent, COMP_TYPE type);
	virtual ~Component();

	virtual void preUpdate(float dt);
	virtual void Update(float dt);
	virtual void Draw();
	virtual void Clear();

	// Component info
	COMP_TYPE GetType() const;
	bool IsActive() const;
	const char* GetName() const;

	// Utility
	virtual bool Enable();
	virtual bool Disable();

private:
	COMP_TYPE type = C_UNKNOWN;
	bool active = true;

protected:
	const char* name = nullptr;
	GameObject* parent = nullptr;
};