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

	if (scene != nullptr && scene->HasMeshes())
	{
		aiNode* rootNode = scene->mRootNode;
		for (int i = 0; i < rootNode->mNumChildren; ++i)
		{
			App->scene_intro->root_gameObjects->AddChild(LoadModel(scene, rootNode->mChildren[i], path));
		}

		// ---- Release resources ----
		aiReleaseImport(scene);
		LOG("---- FBX LOADED WITH SUCCESS ----");
		return true;
	}
	else
		LOG("---- ERROR, COULDN'T LOAD FBX ----");
	return false;
}

GameObject* ModuleImporter::LoadModel(const aiScene* scene, aiNode* node, const char* path)
{
	GameObject* temp_go = new GameObject(nullptr);
	temp_go->SetName((char*)node->mName.C_Str());
	
	if (node->mNumMeshes <= 0)
	{
		LOG("Unable to load the mesh with path: %s. The number of meshes is below or equal to 0.", path);
	}
	else
	{
		// Setting GameObject components
		CompMesh* mesh_comp = new CompMesh(temp_go, COMP_TYPE::C_MESH);
		Mesh* temp_mesh = new Mesh();
		CompMaterial* mat_comp = new CompMaterial(temp_go, COMP_TYPE::C_MATERIAL);

		LOG("Loading mesh from path %s", path);
		for (int i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh* new_mesh = scene->mMeshes[node->mMeshes[i]];
			
			temp_mesh->num_vertices = new_mesh->mNumVertices;
			temp_mesh->vertices = new uint[temp_mesh->num_vertices * 3];
			memcpy(temp_mesh->vertices, new_mesh->mVertices, sizeof(float)*temp_mesh->num_vertices * 3);

			// ---- Generate buffers ----
			glGenBuffers(1, (GLuint*)&(temp_mesh->id_vertices));
			glBindBuffer(GL_ARRAY_BUFFER, temp_mesh->id_vertices);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * temp_mesh->num_vertices, temp_mesh->vertices, GL_STATIC_DRAW);
			
			// ---- Mesh ----
			if (new_mesh->HasFaces())
			{
				temp_mesh->num_indices = new_mesh->mNumFaces * 3;
				temp_mesh->indices = new uint[temp_mesh->num_indices];
				for (uint j = 0; j < new_mesh->mNumFaces; j++)
				{
					if (new_mesh->mFaces[j].mNumIndices == 3)
					{
						memcpy(&temp_mesh->indices[j * 3], new_mesh->mFaces[j].mIndices, 3 * sizeof(uint));
					}
				}

				glGenBuffers(1, (GLuint*)&(temp_mesh->id_indices));
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, temp_mesh->id_indices);
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float) * temp_mesh->num_indices, temp_mesh->indices, GL_STATIC_DRAW);
			}
			else
			{
				LOG("Mesh with %i faces can not be loaded.", new_mesh->mNumFaces);
			}

			// ---- Texture ----
			if (new_mesh->HasTextureCoords(temp_mesh->id_uvs))
			{
				// ---- UVs ----
				temp_mesh->num_uvs = new_mesh->mNumVertices;
				temp_mesh->uvs = new float[temp_mesh->num_uvs * 2];

				for (int i = 0; i < new_mesh->mNumVertices; ++i)
				{
					memcpy(&temp_mesh->uvs[i * 2], &new_mesh->mTextureCoords[0][i].x, sizeof(float));
					memcpy(&temp_mesh->uvs[(i * 2) + 1], &new_mesh->mTextureCoords[0][i].y, sizeof(float));
				}

				glGenBuffers(1, (GLuint*)&(temp_mesh->id_uvs));
				glBindBuffer(GL_ARRAY_BUFFER, temp_mesh->id_uvs);
				glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 2 * temp_mesh->num_uvs, temp_mesh->uvs, GL_STATIC_DRAW);
			}
			else
			{
				LOG("Texture coords couldn´t be found for the specified mesh.");
			}

			// ---- Texture ID ----
			aiMaterial* material = scene->mMaterials[new_mesh->mMaterialIndex];
			if (material)
			{
				aiString path;
				material->GetTexture(aiTextureType_DIFFUSE, 0, &path);

				if (path.length > 0)
				{
					// ---- Find the texture on textures folder ----
					std::string texture_folder = "Textures/";
					std::string final_path = path.data;
					final_path.erase(0, final_path.find_last_of("\\") + 1);
					texture_folder += final_path;

					mat_comp->SetID(CreateTextureID(texture_folder.c_str()));
					LOG("Texture with path %s has been loaded.", texture_folder.c_str());
					final_path.clear();
					texture_folder.clear();
				}
			}

			// ---- Normals ----
			if (new_mesh->HasNormals())
			{
				temp_mesh->num_normals = new_mesh->mNumVertices;
				temp_mesh->normals = new float[temp_mesh->num_normals * 3];
				memcpy(temp_mesh->normals, new_mesh->mNormals, sizeof(float) * temp_mesh->num_normals * 3);

				glGenBuffers(1, (GLuint*)&(temp_mesh->id_normals));
				glBindBuffer(GL_ARRAY_BUFFER, temp_mesh->id_normals);
				glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * temp_mesh->num_normals, temp_mesh->normals, GL_STATIC_DRAW);
			}
			else
			{
				LOG("Mesh has no normals.");
			}
			 
			// ---- Push the mesh ----
			mesh_comp->AddMesh(temp_mesh);
			LOG("Loaded mesh with %i vertices.", temp_mesh->num_vertices);
			LOG("Loaded mesh with %i indices.", temp_mesh->num_indices);
			LOG("Loaded mesh with %i triangles.", temp_mesh->num_vertices / 3);
			LOG("Loaded mesh with %i normals.", temp_mesh->num_normals);
			LOG("Loaded mesh with %i uvs.", temp_mesh->num_uvs);
		}
		temp_go->AddComponent(mesh_comp);
		temp_go->AddComponent(mat_comp);
	}

	for (int i = 0; i < node->mNumChildren; i++)
	{
		temp_go->AddChild(LoadModel(scene, node->mChildren[i], path));
	}

	LOG("GameObject position: (%f, %f, %f)", temp_go->GetCompTransform()->GetPosition().x, temp_go->GetCompTransform()->GetPosition().y, temp_go->GetCompTransform()->GetPosition().z);
	LOG("GameObject rotation: (%f, %f, %f)", temp_go->GetCompTransform()->GetRotation().x, temp_go->GetCompTransform()->GetRotation().y, temp_go->GetCompTransform()->GetRotation().z);
	LOG("GameObject scale: (%f, %f, %f)", temp_go->GetCompTransform()->GetScale().x, temp_go->GetCompTransform()->GetScale().y, temp_go->GetCompTransform()->GetScale().z);
	return temp_go;
}

uint ModuleImporter::CreateTextureID(const char* texture_path)
{
	ILuint id;
	uint texture_id;
	ilGenImages(1, &id);
	ilBindImage(id);
	ilLoadImage(texture_path);

	texture_id = ilutGLBindTexImage();

	return texture_id;
}

void ModuleImporter::CentrateObjectView() const
{
	math::AABB box(float3(0, 0, 0), float3(0, 0, 0));
	box.Enclose((float3*)App->importer->mesh.vertices, App->importer->mesh.num_vertices);

	App->camera->Reference.x = box.CenterPoint().x;
	App->camera->Reference.y = box.CenterPoint().y;
	App->camera->Reference.z = box.CenterPoint().z;

	App->camera->Position.x = box.maxPoint.x * 2; // Increase the distance view
	App->camera->Position.y = box.maxPoint.y * 2;
	App->camera->Position.z = box.maxPoint.z * 2;

	App->camera->LookAt(App->camera->Reference);
}

math::AABB ModuleImporter::GetAABB() const
{
	math::AABB box(float3(0, 0, 0), float3(0, 0, 0));
	box.Enclose((float3*)App->importer->mesh.vertices, App->importer->mesh.num_vertices);

	return box;
}

uint ModuleImporter::GetIndicesQuantity() const
{
	return(mesh.num_indices);
}

uint ModuleImporter::GetVerticesQuantity() const
{
	return(mesh.num_vertices);
}

vec3 ModuleImporter::GetPosition() const
{
	return(mesh.position);
}

vec3 ModuleImporter::GetRotation() const
{
	return(mesh.rotation);
}

vec3 ModuleImporter::GetScale() const
{
	return(mesh.scale);
}

float ModuleImporter::GetNormalsQuantity() const
{
	return(mesh.num_normals);
}

float ModuleImporter::GetUvsQuanity() const
{
	return(mesh.num_uvs);
}

uint ModuleImporter::GetTextureId() const
{
	return(mesh.texture_id);
}