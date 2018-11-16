#ifndef COMPMESH_H
#define COMPMESH_H

#include "Globals.h"
#include <vector>
#include "MathGeoLib/MathGeoLib.h"
#include "ResourceMesh.h"
#include "Component.h"

struct Mesh
{
	uint id_vertices = 0;
	uint num_indices = 0;
	uint* indices = nullptr;

	uint id_indices = 0;
	uint num_vertices = 0;
	float* vertices = nullptr;

	uint id_normals = 0;
	uint num_normals = 0;
	float* normals = nullptr;

	uint id_uvs = 0;
	uint num_uvs = 0;
	float* uvs = nullptr;
};

class GameObject;

class CompMesh : public Component
{
public:
	CompMesh(GameObject* parent, COMP_TYPE type);
	~CompMesh();

	void Update(float dt);
	void Draw();
	void SetMesh(ResourceMesh* mesh);
	void ApplyTexture(const char* path);

	// Read
	uint GetNumIndices()const;
	uint GetNumVertices()const;
	float GetNumNormals()const;
	float GetNumUvs()const;
	math::AABB GetAABB()const;
	uint GetSize();
	void WriteComponentData(char ** cursor);
	float* GetVertices()const;
	Mesh* GetMesh()const;
	// UI 
	void BlitComponentInspector();

public:
	bool wireframe = false;
	float tex_alpha = 0.0f;
	char* binary_path = nullptr;
	ResourceMesh* rmesh = nullptr;
};

#endif // !COMPMESH_H