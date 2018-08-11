#include "Application.h"
#include "Globals.h"
#include "ModuleAbout.h"

ModuleAbout::ModuleAbout(bool start_enabled) : Module(start_enabled)
{}
ModuleAbout::~ModuleAbout()
{}

bool ModuleAbout::Start()
{
	return true;
}
update_status ModuleAbout::Update(float dt)
{
	return UPDATE_CONTINUE;
}
bool ModuleAbout::CleanUp()
{
	return true;
}

//Function that draws all the console
void ModuleAbout::Draw()
{
	ImGui::TextColored(ImVec4(1, 1, 0, 100), "=== PalGine 3D ===");
	ImGui::Text("This is a 3D game engine made by Fran Ruiz and Manel Mourelo,\ntwo college students from the CITM UPC, Terrassa.\nThis engine is made for educational purposes.");
	ImGui::Separator();
	ImGui::TextColored(ImVec4(0, 1, 1, 100), "Project links");
	if (ImGui::MenuItem("- Link to PalmGine 3D repository"))
	{
		ShellExecuteA(0, 0, "chrome.exe", "https://github.com/botttos/PalmGine", 0, SW_SHOWMAXIMIZED);
	}

	if (ImGui::MenuItem("- Link to the latest release"))
	{
		ShellExecuteA(0, 0, "chrome.exe", "https://github.com/botttos/PalmGine/releases", 0, SW_SHOWMAXIMIZED);
	}

	ImGui::Separator();
	ImGui::Text("=== License ===");
	ImGui::TextWrapped("PalmGine 3D is under The MIT License, see LICENSE for more information.");
	if (ImGui::MenuItem("- The MIT License (MIT)"))
	{
		ShellExecuteA(0, 0, "chrome.exe", "https://opensource.org/licenses/mit-license.php", 0, SW_SHOWMAXIMIZED);
	}
}