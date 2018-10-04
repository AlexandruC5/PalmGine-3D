#pragma once
#include "Module.h"
#include "Globals.h"
#include "glmath.h"
#include <vector>
#include "Glew\include\glew.h"
#include "MathGeoLib\Geometry\AABB.h"

class aiScene;
class aiNode;
class aiMaterial;

struct ModelConfig
{
	//Geometry
	uint id_vertices = 0;
	uint num_indices = 0;
	uint* indices = nullptr;

	uint id_indices = 0;
	uint num_vertices = 0;
	uint* vertices = nullptr;

	//Texture
	uint id_normals = 0;
	uint num_normals = 0;
	float* normals = nullptr;

	uint id_uvs = 0;
	uint num_uvs = 0;
	float* uvs = nullptr;

	uint texture_id = 0;

	//Transformation
	vec3 position;
	vec3 rotation;
	vec3 scale;
};

class ModuleImporter : public Module
{
public:
	ModuleImporter(bool start_enabled = true);
	~ModuleImporter();

	// 

	bool Start();
	bool CleanUp();
	
	bool LoadFBX(const char* path);
	GameObject* LoadModel(const aiScene* scene, aiNode* node, const char* path);
	uint CreateTextureID(const char * texture_path);
	void ApplyTexture(const char* path);
	void ClearMeshes();
	void DrawMeshes();

	// Read only
	uint const GetIndices();
	uint const GetVertices();

	vec3 const GetPosition();
	vec3 const GetRotation();
	vec3 const GetScale();

	float const GetNormals();
	float const GetUvs();
	uint const GetTextureId();
	void const CentrateObjectView();
	math::AABB const GetAABB();

public:
	std::string path;
	std::string file_name;
	uint last_texture_id = 0;

	GLfloat fbx_position[3] = { 0.0f, 0.0f, 0.0f };

private:
	//std::vector<ModelConfig> meshes;
	//ModelConfig data;
	ModelConfig mesh;
	
};