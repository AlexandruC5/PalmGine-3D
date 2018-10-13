#include "Application.h"
#include "Globals.h"
#include "PanelConfig.h"
#include "ModuleHardware.h"
#include "ModuleCamera3D.h"
#include "Light.h"
#include "ModuleRenderer3D.h"
#include "MathGeoLib\MathGeoLib.h"
#include "mmgr\mmgr.h"

#define IM_ARRAYSIZE(_ARR)  ((int)(sizeof(_ARR)/sizeof(*_ARR)))

PanelConfig::PanelConfig() : Panel("Config")
{
	memset(InputBuf, 0, sizeof(InputBuf));
	HistoryPos = -1;
	active = true;
}

PanelConfig::~PanelConfig(){}

int PanelConfig::GetFPS() {
	return fpsCap;
}

//Function that draws all the console
void PanelConfig::Draw()
{
	ImGui::Text("Configuration Menu");
	ImGui::Separator();
	ImGui::Separator();
	
	//Aplication
	if (ImGui::CollapsingHeader("Application")) {
		ImGui::TextWrapped("App Name: PalmGine");
		ImGui::TextWrapped("Organization: UPC CITM");
		ImGui::Separator();
		ImGui::Separator();
		//FPS Graph
		for (uint i = 0; i < GRAPH_ARRAY_SIZE; i++)
		{
			fps_array[i] = fps_array[i + 1];
		}
		fps_array[GRAPH_ARRAY_SIZE - 1] = ImGui::GetIO().Framerate;
		char fps_title[25];
		sprintf_s(fps_title, 25, "Framerate %.1f", fps_array[GRAPH_ARRAY_SIZE - 1]);
		ImGui::PlotHistogram("", fps_array, IM_ARRAYSIZE(fps_array), 30, fps_title, 0.0f, 130.0f, ImVec2(0, 80));

		//MS Graph
		for (uint i = 0; i < GRAPH_ARRAY_SIZE; i++)
		{
			ms_array[i] = ms_array[i + 1];
		}
		ms_array[GRAPH_ARRAY_SIZE - 1] = 1000.0f / ImGui::GetIO().Framerate;
		char ms_title[25];
		sprintf_s(ms_title, 25, "Milliseconds %.1f", ms_array[GRAPH_ARRAY_SIZE - 1]);
		ImGui::PlotHistogram("", ms_array, IM_ARRAYSIZE(ms_array), 30, ms_title, 0.0f, 130.0f, ImVec2(0, 80));

		//sM Stats
		sMStats smstats = m_getMemoryStatistics();

		//Memory Graph
		for (uint i = 0; i < GRAPH_ARRAY_SIZE; i++)
		{
			mem_array[i] = mem_array[i + 1];
		}
		mem_array[GRAPH_ARRAY_SIZE - 1] = smstats.totalActualMemory;
		char mem_title[25] = "Memory Consumption";
		//sprintf_s(ms_title, 25, "Memory Consumption %.1f", mem_title[GRAPH_ARRAY_SIZE - 1]);
		ImGui::PlotHistogram("", mem_array, IM_ARRAYSIZE(mem_title), 30, mem_title, 0.0f, 1000000.0f, ImVec2(0, 80));

		//TotalReported memory
		ImGui::Text("Total Reported memory: "); ImGui::SameLine(); ImGui::TextColored(ImVec4(255, 255, 0, 255), "%i", smstats.totalReportedMemory);
		//TotalActual memory
		ImGui::Text("Total actual memory: "); ImGui::SameLine(); ImGui::TextColored(ImVec4(255, 255, 0, 255), "%i", smstats.totalActualMemory);
		//PeakReported memory
		ImGui::Text("Peak reported memory: "); ImGui::SameLine(); ImGui::TextColored(ImVec4(255, 255, 0, 255), "%i", smstats.peakReportedMemory);
		//PeakActual memory
		ImGui::Text("Peak actual memory: "); ImGui::SameLine(); ImGui::TextColored(ImVec4(255, 255, 0, 255), "%i", smstats.peakActualMemory);
		//AccumulatedReported memory
		ImGui::Text("Accumulated reported memory: "); ImGui::SameLine(); ImGui::TextColored(ImVec4(255, 255, 0, 255), "%i", smstats.accumulatedReportedMemory);
		//AccumulatedActual memory
		ImGui::Text("Accumulated actual memory: "); ImGui::SameLine(); ImGui::TextColored(ImVec4(255, 255, 0, 255), "%i", smstats.accumulatedActualMemory);
		//AccumulatedAllocUnit count
		ImGui::Text("Accumulated Alloc Unit Count: "); ImGui::SameLine(); ImGui::TextColored(ImVec4(255, 255, 0, 255), "%i", smstats.accumulatedAllocUnitCount);
		//TotalAllocUnit count
		ImGui::Text("Total Alloc Unit Count: "); ImGui::SameLine(); ImGui::TextColored(ImVec4(255, 255, 0, 255), "%i", smstats.totalActualMemory);
		//PeakAllocUnit count
		ImGui::Text("Peak Alloc Unit Count: "); ImGui::SameLine(); ImGui::TextColored(ImVec4(255, 255, 0, 255), "%i", smstats.peakAllocUnitCount);
	}

	//Window
	if (ImGui::CollapsingHeader("Window")) {
		//Alpha
		ImGuiStyle& style = ImGui::GetStyle();
		ImGui::DragFloat("Global Alpha", &style.Alpha, 0.005f, 0.20f, 1.0f, "%.2f");
		//Brightness
		ImGui::SliderFloat("Brightness", &brightness, 0, 2, NULL);
		//Width
		ImGui::SliderInt("Width", &width, 0, 1920, NULL);
		//Height
		ImGui::SliderInt("Height", &height, 0, 1080, NULL);
		
		//Screen
		if (ImGui::Checkbox("Fullscreen", &App->window->fullscreen))
		{
			App->window->SetFullScreen(&App->window->fullscreen);
		}

		if (ImGui::Checkbox("Windowed", &App->window->windowed))
		{
			App->window->SetWindowed(&App->window->windowed);
		}

		if (ImGui::Checkbox("Full Desktop", &App->window->full_desktop))
		{
			App->window->SetWindowFullDesktop();
			width = 1920;
			height = 1080;
		}

		if (ImGui::Button("Apply"))
		{
			// Window size
			SDL_SetWindowSize(App->window->window, width, height);
			// Brigthness
			SDL_SetWindowBrightness(App->window->window, brightness);
		}

	}

	//Scene
	if (ImGui::CollapsingHeader("Scene")) {
		ImGui::Checkbox("Show grid", &App->scene_intro->grid_enabled);
		ImGui::Checkbox("Show axis", &App->scene_intro->axis_enabled);
	}

	if (ImGui::CollapsingHeader("Render")) 
	{
		ImGui::Checkbox("Wireframe mode", &App->renderer3D->wireframe);
		ImGui::SliderFloat("Alpha tolerance", &App->renderer3D->tex_alpha, 0, 1, NULL);
	}
	//Hardware
	if (ImGui::CollapsingHeader("Hardware")) {
		App->hardware->Draw("Hardware");
	}

	//Camera
	if (ImGui::CollapsingHeader("Camera")) {
		ImGui::SliderFloat("Camera Speed", &App->camera->speed, 0, 15, NULL);
		ImGui::SliderFloat("Wheel Speed", &App->camera->wheelSpeed, 0, 15, NULL);
	}

	//Lights
	if (ImGui::CollapsingHeader("Light")) {
		//on/off
		lightOn = App->renderer3D->lights[0].on;
		ImGui::Checkbox("On/Off", &lightOn);
		//Position
		lightPos[0] = App->renderer3D->lights[0].position.x;
		lightPos[1] = App->renderer3D->lights[0].position.y;
		lightPos[2] = App->renderer3D->lights[0].position.z;
		ImGui::Text("Pos:");
		ImGui::SameLine();
		ImGui::InputFloat3("",lightPos);
		//color
		lightColour[0] = App->renderer3D->lights[0].ambient.r;
		lightColour[1] = App->renderer3D->lights[0].ambient.g;
		lightColour[2] = App->renderer3D->lights[0].ambient.b;
		lightColour[3] = App->renderer3D->lights[0].ambient.a;
		ImGui::Text("Color:");
		ImGui::SameLine();
		ImGui::ColorPicker4("", lightColour, 0);
		//Apply changes
		App->renderer3D->lights[0].Active(lightOn);
		App->renderer3D->lights[0].SetPos(lightPos[0], lightPos[1], lightPos[2]);
		App->renderer3D->lights[0].ambient.Set(lightColour[0], lightColour[1], lightColour[2], lightColour[3]);
		App->renderer3D->lights[0].Init();
	}

	//Input
	if (ImGui::CollapsingHeader("Input")) {
		ImGui::Text("Mouse Positiom: ");
		ImGui::SameLine();
		ImGui::TextColored({ 255, 255, 0, 255 }, "X: %i ", App->input->GetMouseX());
		ImGui::SameLine();
		ImGui::TextColored({ 255, 255, 0, 255 }, "Y: %i", App->input->GetMouseY());

		ImGui::Text("Mouse Motion: ");
		ImGui::SameLine();
		ImGui::TextColored({ 255, 255, 0, 255 }, "X: %i ", App->input->GetMouseXMotion());
		ImGui::SameLine();
		ImGui::TextColored({ 255, 255, 0, 255 }, "Y: %i", App->input->GetMouseYMotion());

		ImGui::Text("Mouse Wheel: ");
		ImGui::SameLine();
		ImGui::TextColored({ 255, 255, 0, 255 }, "%i ", App->input->GetMouseZ());
	}
}