#include "PanelSaveScene.h"
#include "Application.h"

PanelSaveScene::PanelSaveScene() : Panel("Save Scene")
{
	name = new char[64];
	strcpy(name, "scene_name");
}

PanelSaveScene::~PanelSaveScene()
{
	RELEASE_ARRAY(name);
}

void PanelSaveScene::Draw()
{
	ImGui::Begin("Save Scene", &active, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
	ImGui::SetWindowPos({ (float)App->window->height/3, ((float)App->window->height - ((225 * App->window->height) / 1024))/2 });
	ImGui::SetWindowSize(ImVec2(App->window->width/4, (225 * App->window->height) / 1024), ImGuiCond_Always);
	ImGui::Text("Introduce the name of the scene:");
	ImGui::InputText(".binaryscene", name, 64, ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_EnterReturnsTrue);
	
	if (ImGui::Button("save"))
	{
		App->scene_intro->SerializeScene(name);
		active = false;
		saved = true;
	}

	ImGui::End();
}

 const char* PanelSaveScene::GetLastSceneName()
{
	return name;
}
