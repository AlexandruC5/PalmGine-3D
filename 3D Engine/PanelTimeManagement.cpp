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
	ImGui::Begin("Time Management");

	ImGui::SetNextWindowSize(ImVec2(520, 600), ImGuiCond_FirstUseEver);

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