#include "Application.h"
#include "ModuleConsole.h"
#include "ModuleInspector.h"
#include "ModuleConfig.h"
#include "imGUI\imgui.h"
#include "imGUI\imgui_impl_sdl_gl3.h"
#include "Glew\include\glew.h"
#include "ModuleUI.h"

ModuleUI::ModuleUI(bool start_enabled)
{
}

ModuleUI::~ModuleUI()
{
}

bool ModuleUI::Start()
{
	LOG("Loading ImGui");

	glewInit();
	ImGui_ImplSdlGL3_Init(App->window->window);
	App->config->active = false;

	return true;
}

update_status ModuleUI::Update(float dt)
{
	// IMGUI CODE
	static bool show_test_window = false;

	// Test window
	if (show_test_window)
	{
		ImGui::ShowTestWindow();
	}

	// Console
	if (App->console->active == true) {
		App->console->Draw("Console");
	}

	//Inspector/Config menu
	ImGui::Begin("");
	ImGui::SetWindowSize(ImVec2(500, 500), 0);
	if (ImGui::SmallButton("Inspector")) {
		App->config->active = false;
		App->inspector->active = true;
	}
	ImGui::SameLine();
	if (ImGui::SmallButton("Configuration")) {
		App->config->active = true;
		App->inspector->active = false;
	}
	ImGui::Separator();
	//Inspector
	if (App->inspector->active == true) {
		App->inspector->Draw("Inspector");
	}
	if (App->config->active == true) {
		App->config->Draw("Inspector");
	}
	ImGui::End();

	////Hardware
	//if (App->hardware->active == true) {
	//	App->hardware->Draw("Hardware");
	//}

	// Main Bar
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Exit"))
			{
				return UPDATE_STOP;
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Window"))
		{
			if (ImGui::MenuItem("Show test window"))
			{
				if (show_test_window)
				{
					show_test_window = false;
				}
				else
					show_test_window = true;
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Help"))
		{

			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}

	ImGui::Render();
	return UPDATE_CONTINUE;
}

update_status ModuleUI::PreUpdate(float dt)
{
	ImGui_ImplSdlGL3_NewFrame(App->window->window);
	return(UPDATE_CONTINUE);
}

bool ModuleUI::CleanUp()
{
	LOG("Unloading ImGui");
	ImGui_ImplSdlGL3_Shutdown();
	return true;
}
