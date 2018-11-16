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
		if (ImGui::Button(App->ui->assets_path[i].c_str()))
		{
			App->importer->LoadMesh(App->ui->assets_path[i].c_str());
		}
	}
}