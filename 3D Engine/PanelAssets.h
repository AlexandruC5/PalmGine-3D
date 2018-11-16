#ifndef __PanelAssets_H__
#define __PanelAssets_H__

#include "Panel.h"
#include "Globals.h"
#include "imGUI/imgui.h"
#include "Glew/include/glew.h"
#include "SDL/include/SDL_opengl.h"

class PanelAssets : public Panel
{
public:
	PanelAssets();
	~PanelAssets();

	void Draw() override;
	bool active = false;

private:
};

#endif 
