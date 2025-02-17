#ifndef __PanelConfig_H__
#define __PanelConfig_H__

#include "Application.h"
#include "imGUI/imgui.h"
#include "Panel.h"

#define GRAPH_ARRAY_SIZE 70

class PanelConfig : public Panel
{
public:
	PanelConfig();
	~PanelConfig();

	void AddLog(const char*, ...) IM_FMTARGS(2);
	int GetFPS();

	void Draw() override;

public:
	bool active = false;
private:
	int rand = 0;
	int width = 1280;
	int height = 1024;
	int volume = 50;
	float brightness = 1.0;
	int fpsCap = 0;
	bool mute = false;
	float lightColour[4];
	float lightPos[3];
	bool lightOn = true;
	float			fps_array[GRAPH_ARRAY_SIZE];
	float			ms_array[GRAPH_ARRAY_SIZE];
	float			mem_array[GRAPH_ARRAY_SIZE];
	char                  InputBuf[256];
	ImVector<char*>       Items;
	bool                  ScrollToBottom;
	ImVector<char*>       History;
	int                   HistoryPos;    // -1: new line, 0..History.Size-1 browsing history.
	ImVector<const char*> Commands;

	static int   Stricmp(const char* str1, const char* str2) { int d; while ((d = toupper(*str2) - toupper(*str1)) == 0 && *str1) { str1++; str2++; } return d; }
	static char* Strdup(const char *str) { size_t len = strlen(str) + 1; void* buff = malloc(len); return (char*)memcpy(buff, (const void*)str, len); }
};

#endif 
