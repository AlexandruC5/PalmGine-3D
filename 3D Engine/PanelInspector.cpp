#include "Application.h"
#include "Globals.h"
#include "PanelInspector.h"
#include "ModuleImporter.h"
#include "CompTransform.h"
#include "CompMesh.h"

PanelInspector::PanelInspector() : Panel("Inspector")
{}
PanelInspector::~PanelInspector()
{}

//Function that draws all the console
void PanelInspector::Draw()
{
	GameObject* selected_go = App->scene_intro->GetSelectedGO();
	if (selected_go != nullptr)
	{
		ImGui::Text("Inspector");
		ImGui::Separator();
		ImGui::Separator();
		// GO name
		ImGui::Text("Model Name: %s", selected_go->GetName().c_str());
		// Components
		CompTransform* transform = selected_go->GetCompTransform();
		CompMesh* mesh = selected_go->GetCompMesh();
		if (ImGui::CollapsingHeader("Transform"), ImGuiTreeNodeFlags_DefaultOpen) 
		{
			if (transform != nullptr)
			{
				ImGui::Text("Showing read only information about the mesh transform");
				ImGui::Separator();
				//Position
				ImGui::Text("Position:");
				ImGui::Text("[%f]    [%f]    [%f]", transform->GetRotation().x, transform->GetRotation().y, transform->GetRotation().z);
				//Rotation
				ImGui::Text("Rotation:");
				ImGui::Text("[%f]    [%f]    [%f]", transform->GetRotation().x, transform->GetRotation().y, transform->GetRotation().z);
				//Scale
				ImGui::Text("Scale:");
				ImGui::Text("[%f]    [%f]    [%f]", transform->GetScale().x, transform->GetScale().y, transform->GetScale().z);
			}
			else
				LOG("ERROR: COMPONENT TRANSFORM IS NULLPTR on GameObject with name %s", selected_go->GetName().c_str());
		}

		if (mesh != nullptr)
		{
			if (ImGui::CollapsingHeader("Mesh Information"), ImGuiTreeNodeFlags_DefaultOpen)
			{
				ImGui::Text("Showing read only information about the mesh");
				ImGui::Separator();
				//Mesh Triangles
				ImGui::Text("Mesh triangles: %i", mesh->GetNumVertices() / 3);
				//Mesh Vertices
				ImGui::Text("Mesh vertices: %i", mesh->GetNumVertices());
				//Mesh Indices
				ImGui::Text("Mesh indices: %i", mesh->GetNumIndices());
				//Mesh Normals
				ImGui::Text("Mesh normals: %f", mesh->GetNumNormals());
				//Mesh UVS
				ImGui::Text("Mesh uvs: %f", mesh->GetNumUvs());
			}
		}
	}
	
		// TODO Add mesh info
		/*{
		ImGui::Text("Showing read only information about the mesh");
		ImGui::Separator();
		//Mesh Triangles
		ImGui::Text("Mesh triangles: %i", App->fbx->GetVerticesQuantity()/3);
		//Mesh Vertices
		ImGui::Text("Mesh vertices: %i", App->fbx->GetVerticesQuantity());
		//Mesh Indices
		ImGui::Text("Mesh indices: %i", App->fbx->GetIndicesQuantity());
		//Mesh Normals
		ImGui::Text("Mesh normals: %f", App->fbx->GetNormalsQuantity());
		//Mesh UVS
		ImGui::Text("Mesh uvs: %f", App->fbx->GetUvsQuanity());
	}*/

	if (ImGui::CollapsingHeader("Material"), ImGuiTreeNodeFlags_DefaultOpen) {
		//TODO modify this to add textures info in new way
		//ImGui::Text("Showing read only information about the mesh material");
		//ImGui::Separator();
		//ImGui::Text("Texture path: %s", App->fbx->texture_path.c_str());
		//ImGui::Text("Texture WIDTH: %i", App->fbx->textureWidth);
		//ImGui::Text("Texture HEIGHT: %i", App->fbx->textureHeight);
		//if (App->fbx->last_texture_id == 0) {
		//	ImGui::Image((ImTextureID)App->fbx->GetTextureId(), ImVec2(200, 200));
		//}
		//else {
		//	ImGui::Image((ImTextureID)App->fbx->last_texture_id, ImVec2(200, 200));
		//}
	}

	//ImGui::End();
}