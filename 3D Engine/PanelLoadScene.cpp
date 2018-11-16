#include "PanelLoadScene.h"
#include "Application.h"
#include "Globals.h"
#include "PanelSaveScene.h"

PanelLoadScene::PanelLoadScene() : Panel("Save Scene")
{
	name = new char[64];
	strcpy(name, "scene_name");
}

PanelLoadScene::~PanelLoadScene()
{
	RELEASE_ARRAY(name);
}

void PanelLoadScene::Draw()
{
	ImGui::Begin("Load Scene", &active, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
	ImGui::SetWindowPos({ (float)App->window->height / 3, ((float)App->window->height - ((225 * App->window->height) / 1024)) / 2 });
	ImGui::SetWindowSize(ImVec2(App->window->width / 4, (225 * App->window->height) / 1024/2), ImGuiCond_Always);

	if (ImGui::Button("Load last saved scene"))
	{
		if (App->ui->save_scene->saved)
			App->scene_intro->LoadSceneData(App->ui->save_scene->GetLastSceneName());
		else
			LOG("ERROR LOADING SCENE: no scene saved. Please, save the scene before loading it.");
		active = false;
	}
	ImGui::End();
}