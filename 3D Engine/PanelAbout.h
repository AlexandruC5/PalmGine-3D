#ifndef __PanelAbout_H__
#define __PanelAbout_H__

#include "Panel.h"
#include "Globals.h"
#include "imGUI/imgui.h"
#include "Glew/include/glew.h"
#include "SDL/include/SDL_cpuinfo.h"
#include "SDL/include/SDL_opengl.h"

class PanelAbout : public Panel
{
public:
	PanelAbout();
	virtual ~PanelAbout();

	void Draw() override;
};

#endif 
