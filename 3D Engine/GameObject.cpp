#include "GameObject.h"
#include "Globals.h"
#include "CompMesh.h"
#include "CompTransform.h"
#include "CompMaterial.h"
#include "CompCamera.h"
#include "CompAudioListener.h"
#include "CompAudioSource.h"
#include "imGUI/imgui.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Glew/include/glew.h"
#include "ModuleSceneIntro.h"
#include <iostream>

GameObject::GameObject(GameObject* parent) : parent(parent)
{
	uuid = GenRandomNumber();

	this->AddEmptyComponent(COMP_TYPE::C_TRANSFORM);
	if (parent != nullptr)
		parent->childs.push_back(this);
}

GameObject::~GameObject()
{
	parent = nullptr;
	for (uint i = 0; i < childs.size(); i++)
	{
		delete childs[i];
	}
	for (uint i = 0; i < components.size(); i++)
	{
		delete components[i];
	}

	childs.clear();
	components.clear();
}

void GameObject::Update(float dt)
{
	if (active)
	{
		for (uint i = 0; i < components.size(); i++)
		{
			components[i]->Update(dt);
		}
		for (uint i = 0; i < childs.size(); i++)
		{
			childs[i]->Update(dt);
		}

		if (App->scene_intro->selected_gameObject == this)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			glBegin(GL_QUADS);
			glLineWidth(5.0f);
			glColor4f(1.f, 1.f, 0.f, 1.f);

			DebugDrawBox();

			glColor4f(1.f, 1.f, 1.f, 1.f);
			glEnd();
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
	}
}

void GameObject::PopGameObject()
{
	if (parent != nullptr)
	{
		for (uint i = 0; i < parent->childs.size(); i++)
		{
			if (parent->childs[i] == this)
			{
				parent->childs.erase(parent->childs.begin() + i);
				App->scene_intro->to_delete.push_back(this);
				break;
			}
		}
	}
}

void GameObject::SetName(const char* new_name)
{
	name = new_name;
}

std::string GameObject::GetName()
{
	return name;
}

bool GameObject::Enable()
{
	return active = true;
}

bool GameObject::Disable()
{
	return active = false;
}

bool GameObject::IsActive()
{
	return active;
}

bool GameObject::IsRootGo() const
{
	return App->scene_intro->IsRootGO(this);
}

bool GameObject::IsStatic() const 
{
	return static_obj;
}

void GameObject::SetStatic(bool isStatic) 
{
	static_obj = isStatic;
	if (GetNumChilds() > 0) {
		for (int i = 0; i < GetNumChilds(); ++i) {
			childs[i]->SetStatic(isStatic);
		}
	}
}

void GameObject::SetUUID(uint uuid)
{
	this->uuid = uuid;
}

void GameObject::SetParentUUID(uint uuid)
{
	this->parent_uuid = uuid;
}

void GameObject::SetActive(bool set)
{
	active = set;
}

void GameObject::SetUniqueName(const char * name)
{
	SetName(name);
	for (uint i = 0; i < App->scene_intro->root_gameObjects->GetNumChilds(); i++)
	{
		if (strcmp(App->scene_intro->root_gameObjects->childs[i]->GetName().c_str(), name) == 0 && App->scene_intro->root_gameObjects->childs[i] != this)
		{
			std::string tmp_name = name;
			tmp_name += "(0)";
			this->SetName(tmp_name.c_str());
			SetUniqueName(GetName().c_str());
		}
	}
}

const GameObject * GameObject::GetParent() const
{
	return parent;
}

void GameObject::AddChild(GameObject * child)
{
	child->parent = this;
	childs.push_back(child);
}

uint GameObject::GetNumChilds() const
{
	return childs.size();
}

// Components -----------------------

CompTransform* GameObject::GetCompTransform() const
{
	return (CompTransform*)FindComponent(COMP_TYPE::C_TRANSFORM);
}

CompMesh* GameObject::GetCompMesh() const
{
	return (CompMesh*)FindComponent(COMP_TYPE::C_MESH);
}

CompMaterial* GameObject::GetCompMaterial() const
{
	return (CompMaterial*)FindComponent(COMP_TYPE::C_MATERIAL);
}

CompCamera* GameObject::GetCompCamera() const
{
	return (CompCamera*)FindComponent(COMP_TYPE::C_CAMERA);
}

CompAudioListener* GameObject::GetCompAudioListener() const 
{
	return (CompAudioListener*)FindComponent(COMP_TYPE::C_AUDIOLISTENER);
}

CompAudioSource* GameObject::GetCompAudioSource() const
{
	return (CompAudioSource*)FindComponent(COMP_TYPE::C_AUDIO_SOURCE);
}

CompMovement* GameObject::GetCompMovement() const
{
	return  (CompMovement*)FindComponent(COMP_TYPE::C_MOVEMENT);
}

CompReverbZone* GameObject::GetReverbZone() const
{
	return  (CompReverbZone*)FindComponent(COMP_TYPE::C_REVERBZONE);
}

Component * GameObject::AddEmptyComponent(COMP_TYPE type)
{
	if (CompAlreadyExists(type))
	{
		LOG("COMPONENT ALREADY EXITS ON THE GAME OBJECT.");
		return nullptr;
	}
	if (type == COMP_TYPE::C_MESH)
	{
		LOG("Adding MESH COMPONENT to GameObject.");
		CompMesh* mesh = new CompMesh(this, C_MESH);
		components.push_back(mesh);
		return mesh;
	}
	else if (type == COMP_TYPE::C_TRANSFORM)
	{
		LOG("Adding TRANSFORM COMPONENT to GameObject.");
		CompTransform* transform = new CompTransform(this, C_TRANSFORM);
		components.push_back(transform);
		return transform;
	}
	else if (type == COMP_TYPE::C_MATERIAL)
	{
		LOG("Adding MATERIAL COMPONENT to GameObject.");
		CompMaterial* material = new CompMaterial(this, C_MATERIAL);
		components.push_back(material);
		return material;
	}
	
	return nullptr;
}

void GameObject::AddComponent(Component * comp)
{
	if (comp != nullptr)
	{
		if (CompAlreadyExists(comp->GetType()) == false)
		{
			components.push_back(comp);
		}
	}
}

void GameObject::BlitGameObjectHierarchy()
{
	// Check flags
	ImGuiTreeNodeFlags flag = 0;
	if (childs.empty())flag += ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_Leaf;
	// Blit GO name
	char name_str[250];
	sprintf_s(name_str, 250, "%s##%i", name.c_str());
	bool op = ImGui::TreeNodeEx(name_str, flag);

	if (!IsRootGo())
	{
		if (ImGui::IsItemHovered())
		{
			if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN)
			{
				App->scene_intro->SetSelectedGameObject(this);
			}
		}
	}

	// Blit childs
	if (op)
	{
		uint size = childs.size();
		for (uint k = 0; k < size; k++)
		{
			childs[k]->BlitGameObjectHierarchy();
		}

		ImGui::TreePop();
	}
}

Component * GameObject::FindComponent(COMP_TYPE type) const
{
	Component* temp_comp = nullptr;
	for (auto item = components.begin();item != components.end(); item++ )
	{
		if ((*item)->GetType() == type)
		{
			temp_comp = (*item);
			return temp_comp;
		}
	}
	return temp_comp;
}

bool GameObject::CompAlreadyExists(COMP_TYPE type) const
{
	// Checks if component already exists on the GameObject
	for (uint i = 0; i < components.size(); i++)
	{
		if (components[i]->GetType() == type)
		{
			return true;
		}
	}
	return false;
}

void GameObject::DebugDrawBox() 
{
	float3 vertices[8];
	GetAABB().GetCornerPoints(vertices);

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
}

math::AABB GameObject::GetAABB()
{
	math::AABB newbox(float3(-0.25, 0, -0.25), float3(0.25, 0.25, 0.25));
	if (GetCompMesh() == nullptr && childs.size() <= 0) 
	{
		CompTransform* transformation = GetCompTransform();
		OBB boundingBox(newbox);
		boundingBox.Transform(transformation->GetTransformationMatrix());

		newbox = boundingBox.MinimalEnclosingAABB();
	}
	else if(GetCompMesh() != nullptr)
	{
		newbox = GetCompMesh()->GetAABB();
	}

	return newbox;
}

uint GameObject::GetUUID() const
{
	return uuid;
}

uint GameObject::GetParentUUID() const
{
	return parent_uuid;
}
