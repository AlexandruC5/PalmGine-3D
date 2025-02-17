#ifndef __PanelInspector_H__
#define __PanelInspector_H__

#include "Panel.h"
#include "Globals.h"
#include "imGUI/imgui.h"
#include "Glew/include/glew.h"
#include "SDL/include/SDL_opengl.h"

class PanelInspector : public Panel
{
public:
	PanelInspector();
	~PanelInspector();

	void Draw() override;

	void EnableGuizmos(GameObject* selected_go);

public:
	bool active = true;
	bool go_active = true;
};

#endif 
