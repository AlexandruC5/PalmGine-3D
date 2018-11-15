#include "CompMesh.h"
#include "CompMaterial.h"
#include "CompTransform.h"
#include "Component.h"
#include "Application.h"

#include "Devil/include/il.h"
#include "Devil/include/ilut.h"

CompMesh::CompMesh(GameObject * parent, COMP_TYPE type) : Component(parent, type) 
{
	parent->AddComponent(this);
}

CompMesh::~CompMesh()
{
	rmesh->already_loaded--;
	if (rmesh->already_loaded <= 0)
		RELEASE(rmesh);
}

void CompMesh::Update(float dt)
{
	if (this->IsActive())
	{
		if (parent->IsStatic() == false) {
			Draw();
		}
	}
}

void CompMesh::Draw()
{
	CompTransform* trans = parent->GetCompTransform();

	// Wireframe mode
	if (wireframe)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnable(GL_TEXTURE_2D);

	// Adapt blit to geometry
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glMultMatrixf(trans->GetTransformationMatrix().Transposed().ptr());

	glBindBuffer(GL_ARRAY_BUFFER, rmesh->mesh->id_vertices);
	glVertexPointer(3, GL_FLOAT, 0, NULL);
	
	// --- Texture ---
	if (rmesh->mesh->num_uvs > 0)
	{
		glBindBuffer(GL_ARRAY_BUFFER, rmesh->mesh->id_uvs);
		glTexCoordPointer(2, GL_FLOAT, 0, NULL);
	}
	CompMaterial* temp = parent->GetCompMaterial();
	if (temp != nullptr) {
		// Alpha
		glEnable(GL_ALPHA_TEST);
		glAlphaFunc(GL_GREATER, tex_alpha);
		// -- end alpha
		glBindTexture(GL_TEXTURE_2D, temp->GetTextureId());
	}
	else
		LOG("Texture doesn't found. Add a component material to the GameObject and add the texture.");
	// --- End texture ---
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rmesh->mesh->id_indices);
	glDrawElements(GL_TRIANGLES, rmesh->mesh->num_indices, GL_UNSIGNED_INT, NULL);
	
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_ALPHA_TEST);
	glPopMatrix();

	if (wireframe)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

}

void CompMesh::SetMesh(ResourceMesh * mesh)
{
	this->rmesh = mesh;
}

void CompMesh::ApplyTexture(const char * path)
{
	ILuint id;
	ilGenImages(1, &id);
	ilBindImage(id);
	ilLoadImage(path);
	ilutGLBindTexImage();
	LOG("Loaded and applied new texture correctly from path %s.", path);
}

uint CompMesh::GetNumIndices()const
{
	return(rmesh->mesh->num_indices);
}

uint CompMesh::GetNumVertices()const
{
	return(rmesh->mesh->num_vertices);
}

float CompMesh::GetNumNormals()const
{
	return(rmesh->mesh->num_normals);
}

float CompMesh::GetNumUvs()const
{
	return(rmesh->mesh->num_uvs);
}

math::AABB CompMesh::GetAABB()const
{
	CompTransform* transformation = parent->GetCompTransform();
	math::AABB box(float3(0, 0, 0), float3(0, 0, 0));
	box.Enclose((float3*)rmesh->mesh->vertices, rmesh->mesh->num_vertices);
	
	OBB boundingBox(box);
	boundingBox.Transform(transformation->GetTransformationMatrix());	
	box = boundingBox.MinimalEnclosingAABB();

	return box;
}

uint CompMesh::GetSize() 
{
	// GET SIZE FOR SERIALIZATION
	uint size = 0;

	// COMPONENT TYPE
	size += sizeof(int);
	// IS ACTIVE
	size += sizeof(int);
	// BINARY PATH
	size += sizeof(char)*128;

	return size;
}

void CompMesh::WriteComponentData(char ** cursor)
{
	uint bytes = 0;

	// COMPONENT TYPE
	bytes = sizeof(int);
	memcpy(cursor[0], &type, bytes);
	cursor[0] += bytes;
	// IS ACTIVE
	bytes = sizeof(int);
	int tmp_active = (int)active;
	memcpy(cursor[0], &tmp_active, bytes);
	cursor[0] += bytes;
	// BINARY PATH
	bytes = sizeof(char) * 128;
	char* name = new char[128];
	strcpy(name, rmesh->exported_path.c_str());
	memcpy(cursor[0], name, bytes);
	cursor[0] += bytes;
}

float * CompMesh::GetVertices() const
{
	return (rmesh->mesh->vertices);
}

Mesh * CompMesh::GetMesh() const
{
	return rmesh->mesh;
}

void CompMesh::BlitComponentInspector()
{
	ImGui::Separator();

	ImGui::TextColored(ImVec4(1.0f, 0.64f, 0.0f, 1.0f), "Mesh");

	if (rmesh->mesh == nullptr)
		ImGui::Text("NULL MESH RESOURCE");

	else
	{
		//Show num of Vertex
		ImGui::Text("Vertex: %i", GetNumVertices());
	}
}