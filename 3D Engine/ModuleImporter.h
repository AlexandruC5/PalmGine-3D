#ifndef MODULEIMPORTER_H
#define MODULEIMPORTER_H

#include "Module.h"
#include "Globals.h"
#include "glmath.h"
#include <vector>
#include "Glew/include/glew.h"
#include "MathGeoLib/Geometry/AABB.h"

class aiScene;
class aiNode;
class aiMaterial;

class ModuleImporter : public Module
{
public:
	ModuleImporter(bool start_enabled = true);
	~ModuleImporter();

	bool Start();
	bool CleanUp();
	
	bool LoadFBX(const char* path);
	GameObject* LoadModel(const aiScene* scene, aiNode* node, const char* path);
	uint CreateTextureID(const char * texture_path);

public:
	std::string path;
	std::string file_name;

	GLfloat fbx_position[3] = { 0.0f, 0.0f, 0.0f };
};

#endif // !MODULEIMPORTER_H