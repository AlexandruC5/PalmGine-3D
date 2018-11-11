#ifndef __PanelTimeManagement_H__
#define __PanelTimeManagement_H__

#include "Panel.h"
#include "imGUI/imgui.h"

class PanelTimeManagement : public Panel
{
public:
	PanelTimeManagement();
	~PanelTimeManagement();

	void Draw() override;
};

#endif 
