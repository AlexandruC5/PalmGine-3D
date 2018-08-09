#include "Application.h"
#include "Globals.h"
#include "ModuleConfig.h"
#include "ModuleHardware.h"

#define IM_ARRAYSIZE(_ARR)  ((int)(sizeof(_ARR)/sizeof(*_ARR)))

ModuleConfig::ModuleConfig(bool start_enabled) : Module(start_enabled)
{
	memset(InputBuf, 0, sizeof(InputBuf));
	HistoryPos = -1;
}
ModuleConfig::~ModuleConfig()
{}

bool ModuleConfig::Start()
{
	active = true;
	return true;
}
update_status ModuleConfig::Update(float dt)
{
	return UPDATE_CONTINUE;
}
bool ModuleConfig::CleanUp()
{
	return true;
}

int ModuleConfig::GetFPS() {
	return fps_cap;
}

//Function that draws all the console
void ModuleConfig::Draw(const char* title)
{
	ImGui::Text("Config Menu");
	ImGui::Separator();
	ImGui::Separator();
	
	//Aplication
	if (ImGui::CollapsingHeader("Application")) {
		ImGui::TextWrapped("App Name: PalmGine");
		ImGui::TextWrapped("Organization: UPC CITM");
		ImGui::Separator();
		//FPS Graph

	}

	//Window
	if (ImGui::CollapsingHeader("Window")) {

	}

	//Hardware
	if (ImGui::CollapsingHeader("Hardware")) {
		App->hardware->Draw("Hardware");
	}

	//Volume
	if (ImGui::CollapsingHeader("Volume")) {

	}

	//Camera
	if (ImGui::CollapsingHeader("Camera")) {

	}

	//Input
	if (ImGui::CollapsingHeader("Input")) {
		ImGui::Text("Mouse Positiom: ");
		ImGui::SameLine();
		ImGui::TextColored({ 255, 255, 0, 255 }, "X: %i ", App->input->GetMouseX());
		ImGui::SameLine();
		ImGui::TextColored({ 255, 255, 0, 255 }, "Y: %i", App->input->GetMouseY());
	}

}