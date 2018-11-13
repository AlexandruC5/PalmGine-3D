#include "Application.h"
#include "PanelTimeManagement.h"

PanelTimeManagement::PanelTimeManagement() : Panel("TimeManagement")
{
	active = true;
}
PanelTimeManagement::~PanelTimeManagement()
{
}

//Function that draws all the console
void PanelTimeManagement::Draw()
{
	//Creates the console interface
	ImGui::Begin("Time Management", &active, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_HorizontalScrollbar);

	ImGui::SetWindowPos({(250.0f*App->window->width)/1280, 20.0f});
	ImGui::SetWindowSize(ImVec2((500*App->window->width)/1280, (60*App->window->height)/1024), ImGuiCond_Always);

	//Buttons
	if (ImGui::Button("Play")) {
	
	}
	ImGui::SameLine();
	if (ImGui::Button("Pause")) {

	}
	ImGui::SameLine();
	if (ImGui::Button("Stop")) {

	}
	ImGui::SameLine();
	if (ImGui::Button("NextFrame")) {

	}
	ImGui::SameLine();

	//Timer
	float timer = 1.0212;
	ImGui::Text("%.3f", timer);
	ImGui::SameLine();

	//TimeScale Slider
	ImGui::PushItemWidth(150);
	if (ImGui::SliderFloat("Time Scale", &timer, 0.0f, 2.0f, "%.1f")) {

	}

	ImGui::End();
}