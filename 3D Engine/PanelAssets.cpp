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
					std::string tmp = DDS_IMAGES_PATH;
					tmp += textures[i];
					tmp += ".dds";

					uint texture_uuid = App->resource_manager->SearchResource(tmp.c_str());
					if (texture_uuid == 0)
						App->importer->LoadDDS(tmp.c_str(), App->scene_intro->selected_gameObject);
					else
					{
						CompMaterial* mat_comp = App->scene_intro->selected_gameObject->GetCompMaterial();
						mat_comp->rtexture = (ResourceTexture*)App->resource_manager->resources[texture_uuid];
						mat_comp->SetID(mat_comp->rtexture->texture->id, mat_comp->rtexture->exported_path, ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT));
						//mat_comp->rtexture->already_loaded++;
					}
				}
				else
					LOG("GameObject doesn't have component mesh.");
			}
		}
	}
}