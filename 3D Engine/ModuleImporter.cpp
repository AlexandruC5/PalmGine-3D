#include "Application.h"
#include "ModuleImporter.h"
#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Devil/include/il.h"
#include "Devil/include/ilut.h"
#include "MathGeoLib/Geometry/AABB.h"
#include "CompMesh.h"
#include "CompTransform.h"
#include "CompMaterial.h"
#include "ModuleResourceManager.h"
#include "ResourceMesh.h"
#include "ResourceTexture.h"
#include <string>

#pragma comment (lib, "Assimp/libx86/assimp.lib")
#pragma comment (lib, "Devil/libx86/DevIL.lib")
#pragma comment ( lib, "Devil/libx86/ILU.lib" )
#pragma comment ( lib, "Devil/libx86/ILUT.lib" )

using namespace std;

void myCallback(const char *msg, char *userData) {
	LOG("%s", msg);
}

ModuleImporter::ModuleImporter(bool start_enabled) : Module(start_enabled)
{}

ModuleImporter::~ModuleImporter()
{}

bool ModuleImporter::Start()
{
	bool ret = true;

	struct aiLogStream stream = aiLogStream();
	stream.callback = myCallback;
	aiAttachLogStream(&stream);
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);

	ilInit();
	ilutInit();
	ilutRenderer(ILUT_OPENGL);

	return ret;
}

bool ModuleImporter::CleanUp()
{
	aiDetachAllLogStreams();
	return true;
}

bool ModuleImporter::LoadFBX(const char* path)
{
	LOG("Loading FBX...");
	
	const aiScene* scene = aiImportFile(path, aiProcessPreset_TargetRealtime_MaxQuality);
	std::string binary_path = ImportFBX(scene, path, GetFileNameFromPath(path).c_str());
	char* data = LoadData(binary_path.c_str());
	char* cursor = data;
	LoadRecursiveHierarchy(&cursor, App->scene_intro->root_gameObjects);

	return false;
}

std::string ModuleImporter::ImportFBX(const aiScene * scene, const char * path, const char * name)
{
	if (scene != nullptr && scene->HasMeshes())
	{
		return CreateBinary(scene, path, name);
	}
}

std::string ModuleImporter::CreateBinary(const aiScene * scene, const char * path, const char * name)
{
	std::string binary_path;
	//Get scene size to alloc memory
	uint size = BinarySize(scene);
	char* data = nullptr;
	data = new char[size];
	//Create the cursor, wich points to the memory data 
	char* cursor = data;
	//Write in memory
	WriteBinaryRecursive(scene->mRootNode, &cursor, name, scene, path);
	//Write the file
	binary_path = WriteFile(data, name, path, size, ".hierarchy");
	//Create bynary meshes
	CreateBinaryMesh(scene, path);
	//Free the data
	RELEASE_ARRAY(data);

	return binary_path;
}

void ModuleImporter::WriteBinaryRecursive(aiNode * node, char ** cursor, const char * name, const aiScene* scene, const char* path)
{
	aiMesh* mesh;
	uint bytes = 0;
	char mesh_name[64] = {0};
	char mesh_path[128] = {0};
	char texture_name[64] = {0};
	aiVector3D translation = { 0,0,0 };
	aiVector3D scale = { 1,1,1 };
	aiQuaternion rotation = { 0,0,0,0 };
	int num_mesh = 0;
	uint range[4] = { 0, 0, 0, 0 };
	string compare_name = node->mName.C_Str();
	std::string dds_texture_path = "empty_texture";

	bytes = sizeof(uint);
	memcpy(cursor[0], &node->mNumMeshes, bytes);
	cursor[0] += bytes;
	do
	{
		std::string path_name = "empty_mesh";
		if (node->mNumMeshes > 0)
		{
			mesh = scene->mMeshes[node->mMeshes[num_mesh]];

			range[0] = mesh->mNumVertices;
			range[1] = mesh->mNumFaces * 3;
			range[2] = mesh->HasTextureCoords(0);
			range[3] = mesh->HasNormals();

			for (uint i = 0; i < mesh->mNumFaces; i++)
			{
				if (mesh->mFaces[i].mNumIndices != 3)
				{
					LOG("WARNING, geometry face with != 3 faces");
					range[0] = 0;
				}
			}
			aiString tmp_path;
			aiReturn tex_exists = scene->mMaterials[mesh->mMaterialIndex]->GetTexture(aiTextureType_DIFFUSE, 0, &tmp_path);
			if (tex_exists == aiReturn_SUCCESS)
				ImportImage(tmp_path.C_Str());
			strcpy(texture_name, tmp_path.C_Str());
			//Save dds path for texture
			dds_texture_path = DDS_IMAGES_PATH;
			dds_texture_path += GetFileNameFromPath(texture_name);
			dds_texture_path += ".dds";

			path_name = GetFileNameFromPath(path).c_str();
			path_name += "_";
			char* num_path = new char[4];
			itoa(node->mMeshes[num_mesh], num_path, 10);
			path_name += num_path;
			path_name += ".geometry";
		}

		bytes = sizeof(range);
		memcpy(cursor[0], &range, bytes);
		cursor[0] += bytes;
		//Copy mesh name
		bytes = sizeof(char)*64;
		// If set the file name to root node
		if (!compare_name.compare("RootNode"))
			strcpy(mesh_name, GetFileNameFromPath(path).c_str());
		else
			strcpy(mesh_name, node->mName.C_Str());
		memcpy(cursor[0], mesh_name, bytes);
		cursor[0] += bytes;
		//Copy mesh path
		bytes = sizeof(char) * 128;
		strcpy(mesh_path, path_name.c_str());
		memcpy(cursor[0], mesh_path, bytes);
		cursor[0] += bytes;
		//Copy texture name
		bytes = sizeof(char)*64;
		memcpy(cursor[0], dds_texture_path.c_str(), bytes);
		cursor[0] += bytes;
		node->mTransformation.Decompose(scale, rotation, translation);
		//Copy translation
		bytes = sizeof(aiVector3D);
		memcpy(cursor[0], &translation, bytes);
		cursor[0] += bytes;
		//Copy rotation
		bytes = sizeof(aiQuaternion);
		memcpy(cursor[0], &rotation, bytes);
		cursor[0] += bytes;
		//Copy scale
		bytes = sizeof(aiVector3D);
		memcpy(cursor[0], &scale, bytes);
		cursor[0] += bytes;
		//Num childs
		bytes = sizeof(uint);
		memcpy(cursor[0], &node->mNumChildren, bytes);
		cursor[0] += bytes;

		num_mesh++;
	} while (num_mesh < node->mNumMeshes);

	//Recursive for the childs 
	for (uint i = 0; i < node->mNumChildren; i++)
	{
		WriteBinaryRecursive(node->mChildren[i], cursor, name, scene, path);
	}
}

std::string ModuleImporter::WriteFile(char * data, const char * name, const char * path, uint size, char* extension_name)
{
	std::string bin_path;
	FILE * pFile;
	bin_path = BINARY_MESH_PATH;
	bin_path += name;
	bin_path += extension_name;
	if (!FileExist(bin_path.c_str()))
	{
		pFile = fopen(bin_path.c_str(), "wb");
		fwrite(data, sizeof(char), size, pFile);
		fclose(pFile);
	}
	else
	{
		DeleteFileA(bin_path.c_str());
		pFile = fopen(bin_path.c_str(), "wb");
		fwrite(data, sizeof(char), size, pFile);
		fclose(pFile);
	}

	return bin_path;
}

void ModuleImporter::CreateBinaryMesh(const aiScene * scene, const char * path)
{
	for (uint i = 0; i < scene->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[i];
		uint size = 0;
		uint bytes = 0;
		char* data = nullptr;
		char* cursor = nullptr;

		//Num indices
		size += sizeof(uint);
		//Num vertices
		size += sizeof(uint);
		//Text coords
		size += sizeof(int);
		//Num normals
		size += sizeof(int);
		//Faces
		if (mesh->HasFaces())
			size += sizeof(uint)*mesh->mNumFaces * 3;
		//Vertices
		size += sizeof(float)*(mesh->mNumVertices * 3);
		//Coords
		if(mesh->HasTextureCoords(0))
			size += sizeof(float)*(mesh->mNumVertices * 2);
		//Normals
		if(mesh->HasNormals())
			size += sizeof(float)*(mesh->mNumVertices * 3);

		data = new char[size];
		cursor = data;
		//Num indices
		uint num_faces = mesh->mNumFaces * 3;
		bytes = sizeof(uint);
		memcpy(cursor, &num_faces, bytes);
		cursor += bytes;
		//Num vertices
		uint num_vertices = mesh->mNumVertices;
		bytes = sizeof(uint);
		memcpy(cursor, &num_vertices, bytes);
		cursor += bytes;
		//Has text coords
		bytes = sizeof(int);
		int has_tex_coords = mesh->HasTextureCoords(0) ? 1 : 0;
		memcpy(cursor, &has_tex_coords, bytes);
		cursor += bytes;
		//Num normals
		bytes = sizeof(int);
		int has_normals = mesh->HasNormals() ? 1 : 0;
		memcpy(cursor, &has_normals, bytes);
		cursor += bytes;
		//Indices
		//Iterate all faces and check if indices are valid, else clean memory and go next geometry
		bool invalid_geometry = false;
		uint* indices = new uint[mesh->mNumFaces * 3];
		for (uint j = 0; j < mesh->mNumFaces; j++)
		{
			if (mesh->mFaces[j].mNumIndices != 3)
			{
				LOG("WARNING: FACE HAVE != 3 INDICES");
				invalid_geometry = true;
				break;
			}
			else
				memcpy(&indices[j * 3], mesh->mFaces[j].mIndices, sizeof(uint) * 3);
		}
		if (invalid_geometry)
		{
			RELEASE_ARRAY(indices);
			RELEASE_ARRAY(data);
			//Continue with next geometry
			continue;
		}
		bytes = sizeof(uint)*mesh->mNumFaces*3;
		memcpy(cursor, indices, bytes);
		cursor += bytes;
		//Vertices
		bytes = sizeof(float)*mesh->mNumVertices*3;
		memcpy(cursor, mesh->mVertices, bytes);
		cursor += bytes;
		//Texture coords
		if (mesh->HasTextureCoords(0))
		{
			float* new_uvs = new float[num_vertices * 2];

			for (int j = 0; j < num_vertices; ++j)
			{
				memcpy(&new_uvs[j * 2], &mesh->mTextureCoords[0][j].x, sizeof(float));
				memcpy(&new_uvs[(j * 2) + 1], &mesh->mTextureCoords[0][j].y, sizeof(float));
			}

			bytes = sizeof(float)*mesh->mNumVertices * 2;
			memcpy(cursor, new_uvs, bytes);
			cursor += bytes;
		}
		//Normals
		if (mesh->HasNormals())
		{
			bytes = sizeof(float)*mesh->mNumVertices * 3;
			memcpy(cursor, mesh->mNormals, bytes);
			cursor += bytes;
		}
		// Write file
		std::string path_name = GetFileNameFromPath(path).c_str();
		path_name += "_";
		char* num_path = new char[4];
		itoa(i, num_path, 10);
		path_name += num_path;

		WriteFile(data, path_name.c_str(), path, size, ".geometry");
	}
}

uint ModuleImporter::BinarySize(const aiScene * scene)
{
	uint size = 0;
	aiNode* root_node = scene->mRootNode;

	return size = GetRecursiveSize(root_node, scene);
}

uint ModuleImporter::GetRecursiveSize(const aiNode * root_node, const aiScene * scene)
{
	uint size = 0;
	uint iterator = 0;

	do
	{
		uint range[4] = { 0,0,0,0 };
		size += sizeof(range);

		//Position and rotation
		size += sizeof(aiVector3D) * 2 + sizeof(aiQuaternion);
		//Mesh name, mesh path, texture path 
		size += sizeof(char) * 64;
		size += sizeof(char) * 128;
		size += sizeof(char) * 64;
		//Number of childs
		size += sizeof(uint);

		for (int i = 0; i < root_node->mNumChildren; i++)
		{
			size += GetRecursiveSize(root_node->mChildren[i], scene);
		}
		iterator++;
	} while (iterator < root_node->mNumMeshes);
	
	size += sizeof(uint);
	
	return size;
}

GameObject* ModuleImporter::ReadBinaryHierarchy(char** cursor, uint* num_childs, GameObject* parent)
{
	uint bytes = 0;
	uint range[4] = { 0, 0, 0, 0 };
	int num_meshes = 0;
	uint iterator = 0;

	GameObject* go;
	//Iterate all meshes and read them
	bytes = sizeof(uint);
	memcpy(&num_meshes, cursor[0], bytes);
	cursor[0] += bytes;
	do
	{
		//Variables
		char* name = new char[64];
		char* path_name = new char[128];
		char* texture_name = new char[64];
		aiVector3D translation = { 0,0,0 };
		aiVector3D scale = { 0,0,0 };
		aiQuaternion rotation = { 0,0,0,0 };		
		
		//Range
		bytes = sizeof(range);
		memcpy(range, cursor[0], bytes);
		cursor[0] += bytes;
		//Copy mesh name
		bytes = sizeof(char) * 64;
		memcpy(name, cursor[0], bytes);
		cursor[0] += bytes;
		//Copy mesh path
		bytes = sizeof(char) * 128;
		memcpy(path_name, cursor[0], bytes);
		cursor[0] += bytes;
		//Copy texture name
		bytes = sizeof(char) * 64;
		memcpy(texture_name, cursor[0], bytes);
		cursor[0] += bytes;
		//Copy translation
		bytes = sizeof(aiVector3D);
		memcpy(&translation, cursor[0], bytes);
		cursor[0] += bytes;
		//Copy rotation
		bytes = sizeof(aiQuaternion);
		memcpy(&rotation, cursor[0], bytes);
		cursor[0] += bytes;
		//Copy scale
		bytes = sizeof(aiVector3D);
		memcpy(&scale, cursor[0], bytes);
		cursor[0] += bytes;
		//Num childs
		bytes = sizeof(uint);
		memcpy(num_childs, cursor[0], bytes);
		cursor[0] += bytes;

		// *-- CREATE GAME OBJECT --*
		go = new GameObject(parent);
		go->SetName(name);
		CompTransform* transform = go->GetCompTransform();
		transform->SetPosition(float3(translation.x, translation.y, translation.z));
		transform->SetScale(float3(scale.x,scale.y,scale.z));
		transform->SetRotation(float3(rotation.x, rotation.y, rotation.z));
		// Resource mesh
		if (range[0] != 0)
		{
			uint mesh_uuid = App->resource_manager->SearchResource(path_name);
			
			if (mesh_uuid == 0)
				ReadBinaryMesh(path_name, go);
			else
			{
				CompMesh* mesh_comp = new CompMesh(go, COMP_TYPE::C_MESH);
				mesh_comp->rmesh = (ResourceMesh*)App->resource_manager->resources[mesh_uuid];
				mesh_comp->rmesh->already_loaded++;
			}
		}
		// Resource texture
		uint texture_uuid = App->resource_manager->SearchResource(texture_name);

		if (texture_uuid == 0)
			LoadDDS(texture_name, go);
		else
		{
			CompMaterial* mat_comp = new CompMaterial(go, COMP_TYPE::C_MATERIAL);
			mat_comp->rtexture = (ResourceTexture*)App->resource_manager->resources[texture_uuid];
			mat_comp->SetID(mat_comp->rtexture->texture->id, mat_comp->rtexture->exported_path, ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT));
			mat_comp->rtexture->already_loaded++;
		}

		iterator++;
	} while (iterator < num_meshes);
	return go;
}

void ModuleImporter::ReadBinaryMesh(const char * path, GameObject* go)
{
	if (!strcmp(path, "empty_mesh"))
		return;
	CompMesh* mesh_comp = new CompMesh(go, COMP_TYPE::C_MESH);
	uint bytes = 0;
	std::string complete_path = BINARY_MESH_PATH;
	complete_path += path;
	char* data = LoadData(complete_path.c_str());
	char* cursor = data;
	int has_tex_coords = 0;
	int has_normals = 0;
	Mesh* mesh = new Mesh();
	ResourceMesh* rmesh = new ResourceMesh();
	
	mesh_comp->binary_path = (char*)path;
	//Num indices
	bytes = sizeof(uint);
	memcpy(&mesh->num_indices, cursor, bytes);
	cursor += bytes;
	//Num vertices
	bytes = sizeof(uint);
	memcpy(&mesh->num_vertices, cursor, bytes);
	cursor += bytes;
	//Text coords
	bytes = sizeof(int);
	memcpy(&has_tex_coords, cursor, bytes);
	cursor += bytes;
	if (has_tex_coords)
		mesh->num_uvs = mesh->num_vertices;
	else
		mesh->num_uvs = 0;
	//Num normals
	bytes = sizeof(int);
	memcpy(&has_normals, cursor, bytes);
	cursor += bytes;
	if (has_normals)
		mesh->num_normals = mesh->num_vertices;
	else
		mesh->num_normals = 0;
	//Indices
	bytes = sizeof(uint)*mesh->num_indices;
	mesh->indices = new uint[mesh->num_indices];
	memcpy(mesh->indices, cursor, bytes);
	cursor += bytes;
	//Vertices
	bytes = sizeof(float)*mesh->num_vertices*3;
	mesh->vertices = new float[mesh->num_vertices*3];
	memcpy(mesh->vertices, cursor, bytes);
	cursor += bytes;
	//Texture coords
	if (has_tex_coords)
	{
		bytes = sizeof(float)*mesh->num_uvs * 2;
		mesh->uvs = new float[mesh->num_uvs * 2];
		memcpy(mesh->uvs, cursor, bytes);
		cursor += bytes;
	}
	//Normales
	if (has_normals)
	{
		bytes = sizeof(float)*mesh->num_normals * 3;
		mesh->normals = new float[mesh->num_normals * 3];
		memcpy(mesh->normals, cursor, bytes);
		cursor += bytes;
	}
	rmesh->mesh = mesh;
	mesh_comp->SetMesh(rmesh);	
	rmesh->exported_path = path;
	rmesh->already_loaded++;
	App->resource_manager->resources[rmesh->uuid] = (Resource*)rmesh;
	//Gen buffers for geometry
	GenBuffers(mesh_comp);
}

char * ModuleImporter::LoadData(const char * path)
{
	FILE * pFile;
	long lSize;
	char * data;
	size_t result;

	pFile = fopen(path, "rb");
	if (pFile == NULL) { fputs("File error", stderr); exit(1); }
	
	//Obtain file size:
	fseek(pFile, 0, SEEK_END);
	lSize = ftell(pFile);
	rewind(pFile);

	//Allocate memory to contain the whole file:
	data = new char[lSize];// (char*)malloc(sizeof(char)*lSize);
	if (data == NULL) { fputs("Memory error", stderr); exit(2); }

	//Copy the file into the buffer:
	result = fread(data, 1, lSize, pFile);
	if (result != lSize) { fputs("Reading error", stderr); exit(3); }

	fclose(pFile);

	return data;
}

void ModuleImporter::LoadRecursiveHierarchy(char** cursor, GameObject* parent)
{
	uint num_childs = 0;
	GameObject* go = ReadBinaryHierarchy(cursor, &num_childs, parent);

	for (int i = 0; i < num_childs; i++)
	{
		LoadRecursiveHierarchy(cursor, go);
	}
}

void ModuleImporter::GenBuffers(CompMesh * comp_mesh)
{
	Mesh* mesh = comp_mesh->GetMesh();

	glGenBuffers(1, (GLuint*)&(mesh->id_vertices));
	glBindBuffer(GL_ARRAY_BUFFER, mesh->id_vertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * mesh->num_vertices, mesh->vertices, GL_STATIC_DRAW);

	glGenBuffers(1, (GLuint*)&(mesh->id_indices));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->id_indices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float) * mesh->num_indices, mesh->indices, GL_STATIC_DRAW);

	if (mesh->num_uvs > 0)
	{
		glGenBuffers(1, (GLuint*)&(mesh->id_uvs));
		glBindBuffer(GL_ARRAY_BUFFER, mesh->id_uvs);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 2 * mesh->num_uvs, mesh->uvs, GL_STATIC_DRAW);
	}
	else
	{
		LOG("Texture coords couldn´t be found for the specified mesh.");
	}

	if (mesh->num_normals > 0)
	{
		glGenBuffers(1, (GLuint*)&(mesh->id_normals));
		glBindBuffer(GL_ARRAY_BUFFER, mesh->id_normals);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * mesh->num_normals, mesh->normals, GL_STATIC_DRAW);
	}
}

void ModuleImporter::ImportImage(const char * path)
{
	ILuint id;
	uint texture_id;
	ilGenImages(1, &id);
	ilBindImage(id);

	std::string complete_path = TEXTURES_PATH;
	complete_path += path;
	if (ilLoadImage(complete_path.c_str()) == IL_TRUE)
	{
		ILinfo ImageInfo;
		iluGetImageInfo(&ImageInfo);
		std::string name;

		ILuint size;
		ILubyte *data;
		ilSetInteger(IL_DXTC_FORMAT, IL_DXT5);
		size = ilSaveL(IL_DDS, NULL, 0); 
		if (size > 0) 
		{
			data = new ILubyte[size]; //allocate data
			if (ilSaveL(IL_DDS, data, size) > 0) 
			{ 
				FILE * pFile;

				name = DDS_IMAGES_PATH;
				name += GetFileNameFromPath(path);
				name += ".dds";
				if (!FileExist(name.c_str())) 
				{
					pFile = fopen(name.c_str(), "wb");
					fwrite(data, sizeof(char), size, pFile);
					fclose(pFile);
				}
			}
			RELEASE_ARRAY(data);
		}
	}
	texture_id = ilutGLBindTexImage();
}

void ModuleImporter::LoadDDS(const char * path, GameObject* go)
{
	if (!strcmp(path, "empty_texture"))
		return;
	ResourceTexture* rtexture = new ResourceTexture();
	CompMaterial* material = new CompMaterial(go, C_MATERIAL);
	material->rtexture = rtexture;
	rtexture->exported_path = path;
	material->binary_path = (char*)path;
	ILuint id;
	uint texture_id;
	ilGenImages(1, &id);
	ilBindImage(id);
	ilLoadImage(path);
	texture_id = ilutGLBindTexImage();
	material->SetID(texture_id, path, ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT));
	App->resource_manager->resources[rtexture->uuid] = rtexture;
	rtexture->already_loaded++;
}
