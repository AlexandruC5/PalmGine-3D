#include "PanelSaveScene.h"
#include "Application.h"

PanelSaveScene::PanelSaveScene() : Panel("Save Scene")
{}

PanelSaveScene::~PanelSaveScene()
{}

void PanelSaveScene::Draw()
{
	ImGui::Begin("Save Scene", &active, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
	ImGui::SetWindowPos({ (float)App->window->height/3, ((float)App->window->height - ((225 * App->window->height) / 1024))/2 });
	ImGui::SetWindowSize(ImVec2(App->window->width/4, (225 * App->window->height) / 1024), ImGuiCond_Always);
	ImGui::Text("Introduce the name of the scene:");
	char name[50] = "scene_name";
	if (ImGui::InputText(".binaryscene", name, 50, ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_EnterReturnsTrue))
	{
		App->scene_intro->SerializeScene(name);
		active = false;
	}

	ImGui::End();
}
