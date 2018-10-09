#ifndef MODULEFBX_H
#define MODULEFBX_H


#include "Module.h"
#include "Globals.h"
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

class ModuleFBX : public Module
{
public:
	ModuleFBX(bool start_enabled = true);
	~ModuleFBX();

	// 

	bool Start();
	bool CleanUp();
	
	bool LoadFBX(const char* path);
	void LoadModel(const aiScene* scene, aiNode* node, const char* path);
	uint CreateTextureID(const char * texture_path);
	void ApplyTexture(const char* path);
	void ClearMeshes();

	// Read only
	uint GetIndices() const;
	uint GetVertices() const;
	vec3 GetPosition() const;
	vec3 GetRotation() const;
	vec3 GetScale() const;
	float GetNormals() const;
	float GetUvs() const;
	uint GetTextureId() const;
	void CentrateObjectView() const;
	math::AABB GetAABB() const;
	uint MeshesSize() const;

public:
	std::string path;
	std::string file_name;
	uint last_texture_id = 0;

	GLfloat fbx_position[3] = { 0.0f, 0.0f, 0.0f };

private:
	ModelConfig data;
	ModelConfig mesh;
};

#endif // !MODULEFBX_H