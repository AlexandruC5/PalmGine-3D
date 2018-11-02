#ifndef __ModuleConsole_H__
#define __ModuleConsole_H__

#include "Module.h"
#include "Globals.h"
#include "imGUI/imgui.h"

class ModuleConsole : public Module
{
public:
	ModuleConsole(bool start_enabled = false);
	~ModuleConsole();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	void AddLog(const char*, ...) IM_FMTARGS(2);
	void ClearLog();

	void Draw(const char* title);

	bool active = true;
private:


	char                  inputBuf[256];
	ImVector<char*>       items;
	bool                  scrollToBottom;

	static int   Stricmp(const char* str1, const char* str2) { int d; while ((d = toupper(*str2) - toupper(*str1)) == 0 && *str1) { str1++; str2++; } return d; }
	static char* Strdup(const char *str) { size_t len = strlen(str) + 1; void* buff = malloc(len); return (char*)memcpy(buff, (const void*)str, len); }
};

#endif 
