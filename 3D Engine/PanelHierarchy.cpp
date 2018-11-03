#include "Application.h"
#include "Globals.h"
#include "PanelHierarchy.h"
#include "ModuleSceneIntro.h"

PanelHierarchy::PanelHierarchy() : Panel("Inspector")
{}
PanelHierarchy::~PanelHierarchy()
{}

//Function that draws all the console
void PanelHierarchy::Draw()
{
	ImGui::Begin("Game Object Hierarchy", &active,
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoFocusOnAppearing |
		ImGuiWindowFlags_HorizontalScrollbar);
	ImGui::SetWindowSize(ImVec2(500, 500), ImGuiCond_FirstUseEver);
	
	App->scene_intro->root_gameObjects->BlitGameObjectHierarchy();

	//ImGui::TreeNodeEx();

	ImGui::End();
}