#ifndef __PanelLoadScene_H__
#define __PanelLoadScene_H__

#include "Panel.h"
#include "Globals.h"
#include "imGUI/imgui.h"
#include "Glew/include/glew.h"
#include "SDL/include/SDL_opengl.h"
#include <vector>
#include <string>

class PanelLoadScene : public Panel
{
public:
	PanelLoadScene();
	~PanelLoadScene();

	void Draw() override;

	
public:
	std::vector<std::string> scenes;
	bool active = false;
};

#endif 
