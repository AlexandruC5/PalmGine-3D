#include "Application.h"
#include "Panel.h"
#include "PanelAbout.h"
#include "PanelConsole.h"
#include "PanelInspector.h"
#include "PanelConfig.h"
#include "PanelHierarchy.h"
#include "PanelTimeManagement.h"
#include "imGUI/imgui.h"
#include "imGUI/imgui_impl_sdl_gl3.h"
#include "Glew/include/glew.h"
#include "ModuleUI.h"
#include "ImGuizmo-master/ImGuizmo.h"

ModuleUI::ModuleUI(bool start_enabled)
{
	panels.push_back(about = new PanelAbout());
	panels.push_back(console = new PanelConsole());
	panels.push_back(config = new PanelConfig());
	panels.push_back(inspector = new PanelInspector());
	panels.push_back(goHierarchy = new PanelHierarchy());
	panels.push_back(time_management = new PanelTimeManagement());
}

ModuleUI::~ModuleUI()
{
}

bool ModuleUI::Start()
{
	LOG("Loading ImGui");

	glewInit();
	ImGui_ImplSdlGL3_Init(App->window->window);
	config->active = false;

	return true;
}

update_status ModuleUI::Update(float dt)
{
	// IMGUI CODE
	static bool show_test_window = false;
	static bool show_style_editor = false;

	//TimeManagement
	if (time_management->active == true) {
		time_management->Draw();
	}

	// Test window
	if (show_test_window)
	{
		ImGui::ShowTestWindow();
	}

	if (show_style_editor) {
		ImGui::ShowStyleEditor();
	}

	// Console
	if (console->active == true) {
		console->Draw();
	}

	//Inspector/Config menu
	if (configActive == true) {
		ImGui::Begin("", &configActive,ImGuiWindowFlags_NoResize | ImGuiWindowFlags_HorizontalScrollbar);
		ImGui::SetWindowPos({(float) App->window->width - ((500*App->window->width)/1280), 20.0f });
		ImGui::SetWindowSize(ImVec2((500*App->window->width)/1280, (700*App->window->height)/1024), ImGuiCond_Always);
		if (ImGui::SmallButton("Inspector")) {
			config->active = false;
			inspector->active = true;
		}
		ImGui::SameLine();
		
		if (ImGui::SmallButton("Configuration")) {
			config->active = true;
			inspector->active = false;
		}
		ImGui::Separator();
		//Inspector
		if (inspector->active == true) {
			inspector->Draw();
		}
		if (config->active == true) {
			config->Draw();
		}
		ImGui::End();
	}

	//Game Object Hierarchy
	if (goHierarchy->active == true) {
		goHierarchy->Draw();
	}

	// Main Bar
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Exit"))
			{
				return UPDATE_STOP;
			}
			if (ImGui::MenuItem("Save scene")) {
				App->scene_intro->SerializeScene("TEST_SERIALIZED_SCENE");
			}
			if (ImGui::MenuItem("Load scene")) {
				App->scene_intro->LoadSceneData("Assets/Scenes/TEST_SERIALIZED_SCENE.binaryscene");
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
			ImGui::Checkbox("StyleEditor", &show_style_editor);
			ImGui::Checkbox("Inspector/Config", &configActive);
			ImGui::Checkbox("Console", &console->active);
			ImGui::Checkbox("GameObject Hierarchy", &goHierarchy->active);
			ImGui::Checkbox("Time Manager", &time_management->active);
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("About"))
		{
			about->Draw();
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
	//ImGui::NewFrame();
	ImGuizmo::BeginFrame();
	return(UPDATE_CONTINUE);
}

bool ModuleUI::CleanUp()
{

	for (int i = 0; i < panels.size(); i++) {
		panels.at(i)->~Panel();
	}

	panels.clear();

	LOG("Unloading ImGui");
	ImGui_ImplSdlGL3_Shutdown();
	return true;
}
