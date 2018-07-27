#pragma once
#include "Module.h"
#include "Globals.h"
#include "glmath.h"
#include <vector>
#include "Glew\include\glew.h"

class aiScene;
class aiNode;
class aiMaterial;

struct ModelConfig
{
	uint id_vertices = 0;
	uint num_indices = 0;
	uint* indices = nullptr;

	uint id_indices = 0;
	uint num_vertices = 0;
	uint* vertices = nullptr;

	vec3 position;
	vec3 rotation;
	vec3 scale;
};

class ModuleFBX : public Module
{
public:
	ModuleFBX(bool start_enabled = true);
	~ModuleFBX();

	// 

	bool Start();
	update_status Update(float dt);
	bool CleanUp();
	bool LoadFBX(const char* path);
	void LoadModel(const aiScene* scene, aiNode* node, const char* path);
	void ClearMeshes();

	// Read only
	uint const GetIndices();
	uint const GetVertices();
	vec3 const GetPosition();
	vec3 const GetRotation();
	vec3 const GetScale();
	void const CentrateObjectView();

	std::vector<ModelConfig> meshes;

public:
	std::string path;
	std::string file_name;

	GLfloat fbx_position[3] = { 0.0f, 0.0f, 0.0f };

private:
	ModelConfig data;
	ModelConfig mesh;
};