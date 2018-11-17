#include "PanelAssets.h"
#include "Application.h"
#include "Globals.h"

PanelAssets::PanelAssets() : Panel("Save Scene")
{}

PanelAssets::~PanelAssets()
{}

void PanelAssets::Draw()
{
	ImGui::Text("Assets List");
	ImGui::Separator();
	ImGui::Separator();
	if (ImGui::CollapsingHeader("Objects"))
	{
		for (uint i = 0; i < fbx_vector.size(); i++)
		{
			if (ImGui::MenuItem(fbx_vector[i].c_str()))
				App->importer->LoadMesh(fbx_vector[i].c_str());
		}
	}
	if (ImGui::CollapsingHeader("Textures"))
	{
		for (uint i = 0; i < textures.size(); i++)
		{
			if (ImGui::MenuItem(textures[i].c_str()))
			{
				if (App->scene_intro->selected_gameObject != nullptr);
			}
		}
	}
}