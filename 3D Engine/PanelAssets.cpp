#include "PanelAssets.h"
#include "Application.h"
#include "Globals.h"
#include "Devil/include/il.h"

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
			if (ImGui::Button(fbx_vector[i].c_str()))
				App->importer->LoadMesh(fbx_vector[i].c_str());
		}
	}
	if (ImGui::CollapsingHeader("Textures"))
	{
		for (uint i = 0; i < textures.size(); i++)
		{
			if (ImGui::Button(textures[i].c_str()))
			{
				if (App->scene_intro->selected_gameObject != nullptr && App->scene_intro->selected_gameObject->GetCompMaterial() != nullptr)
				{
					App->importer->ApplyAndImportImage(textures[i]);
				}
				else
					LOG("Selected GameObject doesn't have component mesh.");
			}
		}
	}
}