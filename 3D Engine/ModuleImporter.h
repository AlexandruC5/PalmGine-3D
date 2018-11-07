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

	// Binary
	void ImportFBX(const aiScene* scene, const char* path, const char* name);
	void CreateBinary(const aiScene* scene, const char* path, const char* name);
	void WriteBinaryRecursive(aiNode* node, char** cursor, const char* name, const aiScene* scene, const char*path);
	void WriteFileInMemory(char* data, const char* name, const char* path, uint size);
	//Size
	uint BinarySize(const aiScene* scene);
	uint GetRecursiveSize(const aiNode* root_node, const aiScene* scene);
	
public:
	// TODO delete this
	std::string path;
};

#endif // !MODULEIMPORTER_H