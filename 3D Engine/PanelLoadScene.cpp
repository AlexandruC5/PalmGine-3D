#include "PanelLoadScene.h"
#include "Application.h"
#include "Globals.h"
#include "PanelSaveScene.h"

PanelLoadScene::PanelLoadScene() : Panel("Save Scene")
{}

PanelLoadScene::~PanelLoadScene()
{}

void PanelLoadScene::Draw()
{
	ImGui::Begin("Load Scene", &active, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
	ImGui::SetWindowPos({ (float)App->window->height / 3, ((float)App->window->height - ((225 * App->window->height) / 1024)) / 2 });
	ImGui::SetWindowSize(ImVec2(App->window->width / 4, (225 * App->window->height) / 1024), ImGuiCond_Always);
	
	ImGui::Text("List of Scenes");
	ImGui::Separator();
	ImGui::Separator();
	for (uint i = 0; i < scenes.size(); i++)
	{
		if (ImGui::Button(scenes[i].c_str()))
		{
			if (App->scene_intro->game_paused || App->scene_intro->game_running)
			{
				LOG("ERROR LOADING SCENE. Can't load scene on play mode.");
			}
			else
			{
				LOG("LOADING SCENE WITH NAME: %s", scenes[i].c_str());
				App->scene_intro->LoadSceneData(scenes[i].c_str());
				active = false;
			}			
		}
	}
	ImGui::End();
}