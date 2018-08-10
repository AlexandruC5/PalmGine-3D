#include "Application.h"
#include "Globals.h"
#include "ModuleConfig.h"
#include "ModuleHardware.h"
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
	ImGui::Text("Config Menu");
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