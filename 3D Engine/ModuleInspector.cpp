#include "Application.h"
#include "Globals.h"
#include "ModuleInspector.h"
#include "ModuleImporter.h"

ModuleInspector::ModuleInspector(bool start_enabled) : Module(start_enabled)
{}
ModuleInspector::~ModuleInspector()
{}

bool ModuleInspector::Start()
{
	active = true;

	return true;
}
update_status ModuleInspector::Update(float dt)
{
	return UPDATE_CONTINUE;
}
bool ModuleInspector::CleanUp()
{
	return true;
}

//Function that draws all the console
void ModuleInspector::Draw(const char* title)
{
	ImGui::Text("Inspector");
	ImGui::Separator();
	ImGui::Separator();

	ImGui::Text("Model Name: %s", App->fbx->file_name.c_str());
	ImGui::Text("Model Path: %s", App->fbx->path.c_str());

	if (ImGui::CollapsingHeader("Transform"), ImGuiTreeNodeFlags_DefaultOpen) {
		ImGui::Text("Showing read only information about the mesh transform");
		ImGui::Separator();
		//Position
		ImGui::Text("Position:");
		ImGui::Text("[%f]    [%f]    [%f]", App->fbx->GetPosition().x, App->fbx->GetPosition().y, App->fbx->GetPosition().z);
		//Rotation
		ImGui::Text("Rotation:");
		ImGui::Text("[%f]    [%f]    [%f]", App->fbx->GetRotation().x, App->fbx->GetRotation().y, App->fbx->GetRotation().z);
		//Scale
		ImGui::Text("Scale:");
		ImGui::Text("[%f]    [%f]    [%f]", App->fbx->GetScale().x, App->fbx->GetScale().y, App->fbx->GetScale().z);
	}

	if (ImGui::CollapsingHeader("Mesh Information"), ImGuiTreeNodeFlags_DefaultOpen) {
		ImGui::Text("Showing read only information about the mesh");
		ImGui::Separator();
		//Mesh Triangles
		ImGui::Text("Mesh triangles: %i", App->fbx->GetVertices()/3);
		//Mesh Vertices
		ImGui::Text("Mesh vertices: %i", App->fbx->GetVertices());
		//Mesh Indices
		ImGui::Text("Mesh indices: %i", App->fbx->GetIndices());
		//Mesh Normals
		ImGui::Text("Mesh normals: %f", App->fbx->GetNormals());
		//Mesh UVS
		ImGui::Text("Mesh uvs: %f", App->fbx->GetUvs());
	}

	if (ImGui::CollapsingHeader("Material"), ImGuiTreeNodeFlags_DefaultOpen) {
		ImGui::Text("Showing read only information about the mesh material");
		ImGui::Separator();
		if (App->fbx->last_texture_id == 0) {
			ImGui::Image((ImTextureID)App->fbx->GetTextureId(), ImVec2(200, 200));
		}
		else {
			ImGui::Image((ImTextureID)App->fbx->last_texture_id, ImVec2(200, 200));
		}
	}

	//ImGui::End();
}