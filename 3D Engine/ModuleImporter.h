#ifndef MODULEIMPORTER_H
#define MODULEIMPORTER_H

#include "Module.h"
#include "Globals.h"
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

	// 
	bool Start();
	bool CleanUp();
	
	// Utility
	bool LoadFBX(const char* path);
	GameObject* LoadModel(const aiScene* scene, aiNode* node, const char* path);
	uint CreateTextureID(const char * texture_path);
	uint GetTextureId() const;

public:
	std::string path;
	//std::string file_name;
};

#endif // !MODULEIMPORTER_H