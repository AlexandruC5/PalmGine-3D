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

	bool Start();
	bool CleanUp();
	
	// ------ LOAD FILES ------
	bool LoadFBX(const char* path);
	
	// ------ WRITE BINARY ------

	//     -- Mesh --
	std::string ImportFBX(const aiScene* scene, const char* path, const char* name);
	std::string CreateBinary(const aiScene* scene, const char* path, const char* name);
	void WriteBinaryRecursive(aiNode* node, char** cursor, const char* name, const aiScene* scene, const char*path);
	std::string WriteFile(char* data, const char* name, const char* path, uint size, char* extension_name);
	void CreateBinaryMesh(const aiScene* scene, const char* path);
	//     -- Texture --
	void ImportImage(const char* path /*PNG Path*/);

	// ------ READ BINARY ------

	//     -- Mesh --
	GameObject* ReadBinaryHierarchy(char** cursor, uint* num_childs, GameObject* parent);
	void ReadBinaryMesh(const char* path, GameObject* go);
	char* LoadData(const char* path);
	void LoadRecursiveHierarchy(char** cursor, GameObject* parent);
	//     -- Texture --
	void LoadDDS(const char* path /*DDS Path*/, GameObject* go);
	
	// ------ UTILITY ------
	uint BinarySize(const aiScene* scene);
	uint GetRecursiveSize(const aiNode* root_node, const aiScene* scene);
	void GenBuffers(CompMesh* mesh);
};

#endif // !MODULEIMPORTER_H