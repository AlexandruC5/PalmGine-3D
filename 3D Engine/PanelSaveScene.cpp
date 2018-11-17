#include "PanelSaveScene.h"
#include "Application.h"
#include "PanelLoadScene.h"

PanelSaveScene::PanelSaveScene() : Panel("Save Scene")
{
	name = new char[64];
	strcpy(name, "SceneName");
}

PanelSaveScene::~PanelSaveScene()
{
	RELEASE_ARRAY(name);
}

void PanelSaveScene::Draw()
{
	ImGui::Begin("Save Scene", &active, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
	ImGui::SetWindowPos({ (float)App->window->height / 3, ((float)App->window->height - ((225 * App->window->height) / 1024)) / 2 });
	ImGui::SetWindowSize(ImVec2(App->window->width / 4, (225 * App->window->height) / 1024 / 2), ImGuiCond_Always);
	ImGui::Text("Introduce the name of the scene:");
	ImGui::InputText(".binaryscene", name, 64, ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_EnterReturnsTrue);
	
	if (ImGui::Button("save"))
	{
		std::string tmp = name;
		if (OnlyLetters(tmp))
		{
			LOG("SAVING SCENE WITH NAME: %s", name);
			App->scene_intro->SerializeScene(name);
			App->ui->load_scene->scenes.push_back(name);
			active = false;
		}
		else
			LOG("ERROR: INVALID NAME. Scene name only accepts uppercase or lowercase letters.");
	}


	ImGui::End();
}

bool PanelSaveScene::OnlyLetters(std::string name)
{
	for (uint i = 0; i < name.size(); i++)
	{
		uint filter = toupper(name[i]);
		if (filter < 'A' || filter > 'Z')
		{
			return false;
		}
	}
	return true; 
}