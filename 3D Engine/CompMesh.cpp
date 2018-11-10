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
}

void CompMesh::Update(float dt)
{
	if (this->IsActive())
	{
		//if (drawable == true) {
		//	Draw();
		//}
		if (parent->IsStatic() == false) {
			Draw();
		}
	}
}

void CompMesh::Draw()
{
	CompTransform* trans = parent->GetCompTransform();
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnable(GL_TEXTURE_2D);

	// Adapt blit to geometry
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glMultMatrixf(trans->GetTransformationMatrix().Transposed().ptr());

	glBindBuffer(GL_ARRAY_BUFFER, mesh->id_vertices);
	glVertexPointer(3, GL_FLOAT, 0, NULL);
	
	// --- Texture ---
	if (mesh->num_uvs > 0)
	{
		glBindBuffer(GL_ARRAY_BUFFER, mesh->id_uvs);
		glTexCoordPointer(2, GL_FLOAT, 0, NULL);
	}
	CompMaterial* temp = parent->GetCompMaterial();
	if (temp != nullptr)
		glBindTexture(GL_TEXTURE_2D, temp->GetTextureId());
	else
		LOG("Texture doesn't found. Add a component material to the GameObject and add the texture.");
	// --- End texture ---
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->id_indices);
	glDrawElements(GL_TRIANGLES, mesh->num_indices, GL_UNSIGNED_INT, NULL);
	
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}

void CompMesh::SetMesh(Mesh * mesh)
{
	this->mesh = mesh;
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
	return(mesh->num_indices);
}

uint CompMesh::GetNumVertices()const
{
	return(mesh->num_vertices);
}

float CompMesh::GetNumNormals()const
{
	return(mesh->num_normals);
}

float CompMesh::GetNumUvs()const
{
	return(mesh->num_uvs);
}

math::AABB CompMesh::GetAABB()const
{
	CompTransform* transformation = parent->GetCompTransform();
	math::AABB box(float3(0, 0, 0), float3(0, 0, 0));
	box.Enclose((float3*)mesh->vertices, mesh->num_vertices);

	OBB boundingBox(box);
	boundingBox.Transform(transformation->GetTransformationMatrix());

	box = boundingBox.MinimalEnclosingAABB();

	return box;
}

float * CompMesh::GetVertices() const
{
	return (mesh->vertices);
}

Mesh * CompMesh::GetMesh() const
{
	return mesh;
}

void CompMesh::BlitComponentInspector()
{
	ImGui::Separator();

	ImGui::TextColored(ImVec4(1.0f, 0.64f, 0.0f, 1.0f), "Mesh");

	if (mesh == nullptr)
		ImGui::Text("NULL MESH RESOURCE");

	else
	{
		//Show num of Vertex
		ImGui::Text("Vertex: %i", GetNumVertices());
	}
}