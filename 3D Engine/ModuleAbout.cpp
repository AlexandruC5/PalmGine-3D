#include "Application.h"
#include "Globals.h"
#include "ModuleAbout.h"

ModuleAbout::ModuleAbout(bool start_enabled) : Module(start_enabled)
{}
ModuleAbout::~ModuleAbout()
{}

bool ModuleAbout::Start()
{
	return true;
}
update_status ModuleAbout::Update(float dt)
{
	return UPDATE_CONTINUE;
}
bool ModuleAbout::CleanUp()
{
	return true;
}

//Function that draws all the console
void ModuleAbout::Draw()
{
	ImGui::TextColored(ImVec4(1, 1, 0, 100), "=== PalGine 3D ===");
	ImGui::Text("This is a 3D game engine made by Fran Ruiz and Manel Mourelo,\ntwo college students from the CITM UPC, Terrassa.\nThis engine is made for educational purposes.");
	ImGui::Separator();
	ImGui::TextColored(ImVec4(0, 1, 1, 100), "Project links");
	if (ImGui::MenuItem("- Link to PalmGine 3D repository"))
	{
		ShellExecuteA(0, 0, "chrome.exe", "https://github.com/botttos/PalmGine", 0, SW_SHOWMAXIMIZED);
	}

	if (ImGui::MenuItem("- Link to the latest release"))
	{
		ShellExecuteA(0, 0, "chrome.exe", "https://github.com/botttos/PalmGine/releases", 0, SW_SHOWMAXIMIZED);
	}

	ImGui::Separator();
	ImGui::TextColored(ImVec4(0, 1, 0, 100), "=== Libraries used ===");
	if (ImGui::MenuItem("- ImGui (v1.52)"))
	{
		ShellExecuteA(0, 0, "chrome.exe", "https://github.com/ocornut/imgui", 0, SW_SHOWMAXIMIZED);
	}

	if (ImGui::MenuItem("- Brofiler (v1.1.2)"))
	{
		ShellExecuteA(0, 0, "chrome.exe", "http://www.brofiler.com/", 0, SW_SHOWMAXIMIZED);
	}

	if (ImGui::MenuItem("- Parson"))
	{
		ShellExecuteA(0, 0, "chrome.exe", "http://kgabis.github.io/parson/", 0, SW_SHOWMAXIMIZED);
	}

	if (ImGui::MenuItem("- MathGeoLib (v2.0)"))
	{
		ShellExecuteA(0, 0, "chrome.exe", "http://clb.demon.fi/MathGeoLib/nightly/", 0, SW_SHOWMAXIMIZED);
	}

	if (ImGui::MenuItem("- mmgr"))
	{
		ShellExecuteA(0, 0, "chrome.exe", "http://www.flipcode.com/archives/Presenting_A_Memory_Manager.shtml", 0, SW_SHOWMAXIMIZED);
	}

	if (ImGui::MenuItem("- Devil (v1.7.8)"))
	{
		ShellExecuteA(0, 0, "chrome.exe", " http://openil.sourceforge.net", 0, SW_SHOWMAXIMIZED);
	}

	ImGui::AlignFirstTextHeightToWidgets();
	ImGui::Text("You can download it from"); ImGui::SameLine();
	if (ImGui::Button("here", ImVec2(40, 20)))
	{
		ShellExecuteA(0, 0, "chrome.exe", "http://www.stratos-ad.com/forums/index.php?topic=9897.0", 0, SW_SHOWMAXIMIZED);
	}
	ImGui::Text("Just look for the first comment from the user:"); ImGui::SameLine();
	ImGui::TextColored(ImVec4(1, 1, 0, 100), "Astat");

	if (ImGui::MenuItem("- Bullet (v2.84)"))
	{
		ShellExecuteA(0, 0, "chrome.exe", "http://bulletphysics.org/wordpress/", 0, SW_SHOWMAXIMIZED);
	}

	if (ImGui::MenuItem("- Assimp (v3.3.1)"))
	{
		ShellExecuteA(0, 0, "chrome.exe", "http://assimp.sourceforge.net/", 0, SW_SHOWMAXIMIZED);
	}

	if (ImGui::MenuItem("- Glew (v2.0.0)"))
	{
		ShellExecuteA(0, 0, "chrome.exe", "http://glew.sourceforge.net/", 0, SW_SHOWMAXIMIZED);
	}
	if (ImGui::MenuItem("- GLUT (v3.7)"))
	{
		ShellExecuteA(0, 0, "chrome.exe", "https://www.opengl.org/resources/libraries/glut/", 0, SW_SHOWMAXIMIZED);
	}

	if (ImGui::MenuItem("- SDL (v2.0.6)"))
	{
		ShellExecuteA(0, 0, "chrome.exe", "https://www.libsdl.org/", 0, SW_SHOWMAXIMIZED);
	}

	if (ImGui::MenuItem("- SDL_mixer (v2.0.0)"))
	{
		ShellExecuteA(0, 0, "chrome.exe", "https://www.libsdl.org/projects/SDL_mixer/", 0, SW_SHOWMAXIMIZED);
	}
	ImGui::Separator();
	ImGui::Text("=== License ===");
	ImGui::TextWrapped("PalmGine 3D is under The MIT License, see LICENSE for more information.");
	if (ImGui::MenuItem("- The MIT License (MIT)"))
	{
		ShellExecuteA(0, 0, "chrome.exe", "https://opensource.org/licenses/mit-license.php", 0, SW_SHOWMAXIMIZED);
	}

	ImGui::Separator();
	if (ImGui::BeginMenu("- OpenGL info"))
	{
		ImGui::Text("Vendor: ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(0, 1, 0, 100), "%s", glGetString(GL_VENDOR));

		ImGui::Text("Renderer: ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(0, 1, 0, 100), "%s", glGetString(GL_RENDERER));

		ImGui::Text("OpenGL version supported: ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(0, 1, 0, 100), "%s", glGetString(GL_VERSION));

		ImGui::Text("GLSL: ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(0, 1, 0, 100), "%s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

		ImGui::EndMenu();
	}
}