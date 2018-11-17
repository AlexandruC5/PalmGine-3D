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
	for (uint i = 0; i < App->ui->assets_path.size(); i++)
	{
		if (ImGui::MenuItem(GetFileNameFromPath(App->ui->assets_path[i].c_str()).c_str()))
		{
			if (ImGui::IsItemClicked())
				App->importer->LoadMesh(App->ui->assets_path[i].c_str());

			if (ImGui::IsItemHovered())
			{
				ImGui::BeginTooltip();
				ImGui::Text("Source: %s", App->ui->assets_path[i].c_str());
				ImGui::EndTooltip();
			}
		}
	}
}