#include "Application.h"
#include "Panel.h"
#include "PanelAbout.h"
#include "PanelConsole.h"
#include "PanelInspector.h"
#include "PanelConfig.h"
#include "PanelHierarchy.h"
#include "PanelTimeManagement.h"
#include "PanelSaveScene.h"
#include "PanelLoadScene.h"
#include "PanelAssets.h"
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
	panels.push_back(save_scene = new PanelSaveScene());
	panels.push_back(load_scene = new PanelLoadScene());
	panels.push_back(assets = new PanelAssets());

	ImGuiStyle& style = ImGui::GetStyle();
	style.Colors[ImGuiCol_Text] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.72f, 0.72f, 0.72f, 0.93f);
	style.Colors[ImGuiCol_ChildWindowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	style.Colors[ImGuiCol_PopupBg] = ImVec4(0.88f, 0.88f, 0.88f, 0.90f);
	style.Colors[ImGuiCol_Border] = ImVec4(0.59f, 0.59f, 0.59f, 0.40f);
	style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	style.Colors[ImGuiCol_FrameBg] = ImVec4(0.35f, 0.35f, 0.35f, 0.65f);
	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.22f, 0.22f, 0.22f, 0.40f);
	style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.81f, 0.81f, 0.81f, 0.45f);
	style.Colors[ImGuiCol_TitleBg] = ImVec4(0.54f, 0.54f, 0.54f, 0.83f);
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.73f, 0.73f, 0.73f, 0.60f);
	style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.40f, 0.40f, 0.40f, 0.87f);
	style.Colors[ImGuiCol_MenuBarBg] = ImVec4(1.00f, 1.00f, 1.00f, 0.80f);
	style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.34f, 0.34f, 0.34f, 0.60f);
	style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.75f, 0.75f, 0.75f, 0.82f);
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(1.00f, 1.00f, 1.00f, 0.78f);
	style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	style.Colors[ImGuiCol_ComboBg] = ImVec4(0.20f, 0.20f, 0.20f, 0.99f);
	style.Colors[ImGuiCol_CheckMark] = ImVec4(0.90f, 0.90f, 0.90f, 0.50f);
	style.Colors[ImGuiCol_SliderGrab] = ImVec4(1.00f, 1.00f, 1.00f, 0.30f);
	style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	style.Colors[ImGuiCol_Button] = ImVec4(0.85f, 0.85f, 0.85f, 1.00f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.91f, 0.91f, 0.91f, 1.00f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	style.Colors[ImGuiCol_Header] = ImVec4(0.87f, 0.87f, 0.87f, 0.45f);
	style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.94f, 0.94f, 0.94f, 0.80f);
	style.Colors[ImGuiCol_HeaderActive] = ImVec4(1.00f, 1.00f, 1.00f, 0.98f);
	style.Colors[ImGuiCol_Separator] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
	style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.60f, 0.60f, 0.70f, 1.00f);
	style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.70f, 0.70f, 0.90f, 1.00f);
	style.Colors[ImGuiCol_ResizeGrip] = ImVec4(1.00f, 1.00f, 1.00f, 0.30f);
	style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(1.00f, 1.00f, 1.00f, 0.60f);
	style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(1.00f, 1.00f, 1.00f, 0.90f);
	style.Colors[ImGuiCol_CloseButton] = ImVec4(0.88f, 0.88f, 0.88f, 0.80f);
	style.Colors[ImGuiCol_CloseButtonHovered] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	style.Colors[ImGuiCol_CloseButtonActive] = ImVec4(0.70f, 0.70f, 0.70f, 1.00f);
	style.Colors[ImGuiCol_PlotLines] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.49f, 0.61f, 0.83f, 1.00f);
	style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.42f, 0.66f, 0.82f, 1.00f);
	style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.35f);
	style.Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(0.29f, 0.20f, 0.20f, 0.35f);
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
	App->ui->load_scene->scenes = ReadAllFilesOnPath(BINARY_SCENE_PATH, ".binaryscene");
	App->ui->assets->fbx_vector = ReadAllFilesOnPath(BINARY_MESH_PATH, ".hierarchy");
	App->ui->assets->textures = ReadAllFilesOnPath(DDS_IMAGES_PATH, ".dds");
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
		ImGui::Begin("", &configActive,ImGuiWindowFlags_NoResize | ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_NoMove);
		ImGui::SetWindowPos({(float) App->window->width - ((500*App->window->width)/1280), 20.0f });
		ImGui::SetWindowSize(ImVec2((500*App->window->width)/1280, (700*App->window->height)/1024), ImGuiCond_Always);
		if (ImGui::SmallButton("Inspector")) {
			config->active = false;
			inspector->active = true;
			assets->active = false;
		}
		ImGui::SameLine();
		if (ImGui::SmallButton("Assets")) {
			assets->active = true;
			inspector->active = false;
			config->active = false;
		}
		ImGui::SameLine();
		
		if (ImGui::SmallButton("Configuration")) {
			config->active = true;
			inspector->active = false;
			assets->active = false;
		}
		ImGui::Separator();
		//Inspector
		if (inspector->active == true) {
			inspector->Draw();
		}
		if (config->active == true) {
			config->Draw();
		}
		if (assets->active == true) {
			assets->Draw();
		}
		ImGui::End();
	}

	//Game Object Hierarchy
	if (goHierarchy->active == true) {
		goHierarchy->Draw();
	}
	//Save scene
	if (save_scene != nullptr && save_scene->active == true)
		save_scene->Draw();
	//Load scene
	if (load_scene != nullptr && load_scene->active == true)
		load_scene->Draw();
	// Main Bar
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Exit"))
			{
				return UPDATE_STOP;
			}
			if (ImGui::MenuItem("Save scene")) 
			{
				save_scene->active = true;
				load_scene->active = false;
			}
			if (ImGui::MenuItem("Load scene")) 
			{
				load_scene->active = true;
				save_scene->active = false;
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
