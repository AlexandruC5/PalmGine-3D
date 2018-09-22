#include "Application.h"
#include "Globals.h"
#include "ModuleConfig.h"
#include "ModuleHardware.h"
#include "ModuleCamera3D.h"
#include "Light.h"
#include "ModuleRenderer3D.h"
#include "MathGeoLib\MathGeoLib.h"
#include "mmgr\mmgr.h"

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
	return fpsCap;
}

//Function that draws all the console
void ModuleConfig::Draw(const char* title)
{
	ImGui::Text("Configuration Menu");
	ImGui::Separator();
	ImGui::Separator();
	
	//Aplication
	if (ImGui::CollapsingHeader("Application")) {
		ImGui::TextWrapped("App Name: PalmGine");
		ImGui::TextWrapped("Organization: UPC CITM");
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

		//Acummulated memory
		ImGui::Text("Accumulated actual memory: "); ImGui::SameLine(); ImGui::TextColored(ImVec4(255, 255, 0, 255), "%i", smstats.accumulatedActualMemory);
		//Memory peak
		ImGui::Text("Actual memory peak: "); ImGui::SameLine(); ImGui::TextColored(ImVec4(255, 255, 0, 255), "%i", smstats.peakActualMemory);
		//Actual memory
		ImGui::Text("Total actual memory: "); ImGui::SameLine(); ImGui::TextColored(ImVec4(255, 255, 0, 255), "%i", smstats.totalActualMemory);
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

	//Hardware
	if (ImGui::CollapsingHeader("Hardware")) {
		App->hardware->Draw("Hardware");
	}

	//Volume
	if (ImGui::CollapsingHeader("Volume")) {
		ImGui::SliderInt("Volume", &volume, 0, 100, NULL);
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
		ImGui::ColorEdit4("", lightColour,0);
		//Apply changes
		App->renderer3D->lights[0].Active(lightOn);
		App->renderer3D->lights->SetPos(lightPos[0], lightPos[1], lightPos[2]);
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
	}
}