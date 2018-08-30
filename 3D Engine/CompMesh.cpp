#include "CompMesh.h"
#include "CompMaterial.h"
#include "Component.h"
#include "Application.h"

CompMesh::CompMesh(GameObject * parent, COMP_TYPE type) : Component(parent, type) 
{
}

CompMesh::~CompMesh()
{
}

void CompMesh::Update(float dt)
{
	if (this->IsActive())
	{
		Draw();
	}
}

void CompMesh::Draw()
{
	for (std::vector<Mesh>::iterator item = meshes.begin(); item != meshes.end(); ++item)
	{
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glEnable(GL_TEXTURE_2D);

		glBindBuffer(GL_ARRAY_BUFFER, mesh.id_vertices);
		glVertexPointer(3, GL_FLOAT, 0, NULL);

		// --- Texture ---
		glBindBuffer(GL_ARRAY_BUFFER, mesh.id_uvs);
		glTexCoordPointer(2, GL_FLOAT, 0, NULL);
		CompMaterial* temp = parent->GetCompMaterial();
		if (temp != nullptr)
			glBindTexture(GL_TEXTURE_2D, temp->GetTextureId());
		else
			LOG("Texture doesn't found. Add a component material to the GameObject and add the texture.");
		// --- End texture ---

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.id_indices);
		glDrawElements(GL_TRIANGLES, mesh.num_indices, GL_UNSIGNED_INT, NULL);

		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glBindTexture(GL_TEXTURE_2D, 0);
		glDisable(GL_TEXTURE_2D);
	}
}

uint const CompMesh::GetIndices()
{
	return(mesh.num_indices);
}

uint const CompMesh::GetVertices()
{
	return(mesh.num_vertices);
}

float const CompMesh::GetNormals()
{
	return(mesh.num_normals);
}

float const CompMesh::GetUvs()
{
	return(mesh.num_uvs);
}

math::AABB const CompMesh::GetAABB()
{
	math::AABB box(float3(0, 0, 0), float3(0, 0, 0));
	box.Enclose((float3*)mesh.vertices, mesh.num_vertices);

	return box;
}

uint const CompMesh::GetMeshesSize()
{
	return meshes.size();
}