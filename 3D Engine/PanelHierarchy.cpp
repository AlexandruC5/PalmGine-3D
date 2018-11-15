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
		ImGuiWindowFlags_HorizontalScrollbar | 
		ImGuiWindowFlags_NoMove);

	ImGui::SetWindowPos({ 0.0f, 20.0f });
	ImGui::SetWindowSize(ImVec2((225*App->window->width)/1280, (500*App->window->height)/1024), ImGuiCond_Always);
	
	App->scene_intro->root_gameObjects->BlitGameObjectHierarchy();

	ImGui::End();
}