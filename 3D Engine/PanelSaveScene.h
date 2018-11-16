#ifndef __PanelSaveScene_H__
#define __PanelSaveScene_H__

#include "Panel.h"
#include "Globals.h"
#include "imGUI/imgui.h"
#include "Glew/include/glew.h"
#include "SDL/include/SDL_opengl.h"

class PanelSaveScene : public Panel
{
public:
	PanelSaveScene();
	~PanelSaveScene();

	void Draw() override;
	const char* GetLastSceneName();
	bool active = false;
	bool saved = false;
private:
	char* name = nullptr;
};

#endif 
