#include "Application.h"
#include "Globals.h"
#include "PanelAbout.h"

PanelAbout::PanelAbout() : Panel("About")
{
	active = false;
}

PanelAbout::~PanelAbout()
{}


//Function that draws all the console
void PanelAbout::Draw()
{
	ImGui::TextColored({ 0, 0, 0, 255 }, "=== PalGine 3D ===");
	ImGui::Text("Palmgine 3D is a project consistent of creating a Game Engine.\nAt this moment, you can import your model (.fbx, .obj, etc),\nload geometry and change or apply one texture on the model (.jpg or .png).\nDrop the files on the window.\nHope you like it.");
	ImGui::Separator();
	ImGui::TextColored({ 0, 0, 0, 255 }, "Project links");
	if (ImGui::MenuItem("- Link to PalmGine 3D repository"))
	{
		ShellExecuteA(0, 0, "chrome.exe", "https://github.com/botttos/PalmGine", 0, SW_SHOWMAXIMIZED);
	}

	if (ImGui::MenuItem("- Link to the latest release"))
	{
		ShellExecuteA(0, 0, "chrome.exe", "https://github.com/botttos/PalmGine/releases", 0, SW_SHOWMAXIMIZED);
	}
	if (ImGui::MenuItem("- Report a Bug"))
	{
		ShellExecuteA(0, 0, "chrome.exe", "https://github.com/botttos/PalmGine-3D/issues", 0, SW_SHOWMAXIMIZED);
	}

	ImGui::Separator();
	ImGui::Text("=== License ===");
	ImGui::TextWrapped("PalmGine 3D is under Apache 2.0 License, see LICENSE for more information.");
	if (ImGui::MenuItem("- Apache License 2.0"))
	{
		ShellExecuteA(0, 0, "chrome.exe", "http://www.apache.org/licenses/LICENSE-2.0", 0, SW_SHOWMAXIMIZED);
	}

	ImGui::Separator();

	ImGui::Text("Libraries used");
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

	if (ImGui::MenuItem("- MathGeoLib (v1.5)"))
	{
		ShellExecuteA(0, 0, "chrome.exe", "http://clb.demon.fi/MathGeoLib/nightly/", 0, SW_SHOWMAXIMIZED);
	}

	if (ImGui::MenuItem("- mmgr"))
	{
		ShellExecuteA(0, 0, "chrome.exe", "http://www.flipcode.com/archives/Presenting_A_Memory_Manager.shtml", 0, SW_SHOWMAXIMIZED);
	}

	if (ImGui::MenuItem("- Devil (v1.7.8). Download on the Astat's comment."))
	{
		ShellExecuteA(0, 0, "chrome.exe", " http://openil.sourceforge.net", 0, SW_SHOWMAXIMIZED);
		ShellExecuteA(0, 0, "chrome.exe", "http://www.stratos-ad.com/forums/index.php?topic=9897.0", 0, SW_SHOWMAXIMIZED);
	}

	if (ImGui::MenuItem("- Assimp (v3.3.1)"))
	{
		ShellExecuteA(0, 0, "chrome.exe", "http://assimp.sourceforge.net/", 0, SW_SHOWMAXIMIZED);
	}

	if (ImGui::MenuItem("- Glew (v2.0.0)"))
	{
		ShellExecuteA(0, 0, "chrome.exe", "http://glew.sourceforge.net/", 0, SW_SHOWMAXIMIZED);
	}

	if (ImGui::MenuItem("- SDL (v2.0.6)"))
	{
		ShellExecuteA(0, 0, "chrome.exe", "https://www.libsdl.org/", 0, SW_SHOWMAXIMIZED);
	}
	if (ImGui::MenuItem("- WWISE (v2017.2.6.6636)"))
	{
		ShellExecuteA(0, 0, "chrome.exe", "https://www.audiokinetic.com/library/2017.2.8_6698/?source=SDK&id=index.html", 0, SW_SHOWMAXIMIZED);
	}


	if (ImGui::BeginMenu("- OpenGL info"))
	{
		ImGui::Text("Vendor: ");
		ImGui::SameLine();
		ImGui::TextColored({ 125, 156, 212, 255 }, "%s", glGetString(GL_VENDOR));

		ImGui::Text("Renderer: ");
		ImGui::SameLine();
		ImGui::TextColored({ 125, 156, 212, 255 }, "%s", glGetString(GL_RENDERER));

		ImGui::Text("OpenGL version supported: ");
		ImGui::SameLine();
		ImGui::TextColored({ 125, 156, 212, 255 }, "%s", glGetString(GL_VERSION));

		ImGui::Text("GLSL: ");
		ImGui::SameLine();
		ImGui::TextColored({ 125, 156, 212, 255 }, "%s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

		ImGui::EndMenu();
	}
	ImGui::Separator();
	ImGui::Spacing();
	// Name of the Author
	ImGui::Text("Authors:");
	ImGui::Spacing();
	ImGui::Bullet(); 
	ImGui::SameLine();
	if (ImGui::MenuItem("Fran Ruiz Gimenez"))
	{
		ShellExecuteA(0, 0, "chrome.exe", "https://github.com/botttos", 0, SW_SHOWMAXIMIZED);
	}
	ImGui::Bullet(); ImGui::SameLine();
	if (ImGui::MenuItem("Manel Mourelo Montero"))
	{
		ShellExecuteA(0, 0, "chrome.exe", "https://github.com/manelmourelo", 0, SW_SHOWMAXIMIZED);
	}
}