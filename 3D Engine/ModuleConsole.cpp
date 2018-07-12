#include "Application.h"
#include "Globals.h"
#include "ModuleConsole.h"

#define IM_ARRAYSIZE(_ARR)  ((int)(sizeof(_ARR)/sizeof(*_ARR)))

ModuleConsole::ModuleConsole(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	ClearLog();
	memset(inputBuf, 0, sizeof(inputBuf));
	AddLog("Welcome to ImGui!");
}
ModuleConsole::~ModuleConsole()
{}

bool ModuleConsole::Start()
{
	active = true;
	return true;
}
update_status ModuleConsole::Update(float dt)
{
	return UPDATE_CONTINUE;
}
bool ModuleConsole::CleanUp()
{
	ClearLog();
	return true;
}

//Function to add a new log to the console
void ModuleConsole::AddLog(const char* fmt, ...) IM_FMTARGS(2)
{
	items.push_back(Strdup(fmt));
	scrollToBottom = true;
}

//Function that delete all the logs in the console
void ModuleConsole::ClearLog()
{
	if (items.Size > 0) {
		for (int i = 0; i < items.Size; i++) {
			free(items[i]);
		}
	}
	items.clear();
	scrollToBottom = true;
}

//Function that draws all the console
void ModuleConsole::Draw(const char* title)
{
	//Creates the console interface
	ImGui::Begin(title);

	ImGui::SetNextWindowSize(ImVec2(520, 600), ImGuiCond_FirstUseEver);

	if (ImGui::SmallButton("Clear")) {
		ClearLog();
	}
	ImGui::SameLine();
	if (ImGui::SmallButton("Scrool to bottom")) {
		scrollToBottom = true;
	}
	ImGui::Separator();
	
	ImGui::BeginChild(("Scroll"), ImVec2{0,-ImGui::GetItemsLineHeightWithSpacing() },false, ImGuiWindowFlags_HorizontalScrollbar);
	if (ImGui::BeginPopupContextWindow()) {
		if (ImGui::Selectable("Clear")) {
			ClearLog();
		}
		ImGui::EndPopup();
	}

	//Write logs
	for (int i = 0; i < items.Size; i++) {
		const char* currentItem = items[i];
		ImVec4 colour = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
		if (strstr(currentItem, "[error]")) {
			colour = ImColor(1.0f, 0.4f, 0.4f, 1.0f);
		}
		else if (strncmp(currentItem, "# ", 2) == 0) {
			colour = ImColor(1.0f, 0.78f, 0.58f, 1.0f);
		}
		ImGui::PushStyleColor(ImGuiCol_Text, colour);
		ImGui::TextUnformatted(currentItem);
		ImGui::PopStyleColor();
	}
	if (scrollToBottom == true) {
		ImGui::SetScrollHere();
	}
	scrollToBottom = false;

	ImGui::EndChild();

	ImGui::End();
}