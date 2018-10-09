#include "UIPanel.h"

UiPanel::UiPanel(const char* name) : name(name) {

}
UiPanel::~UiPanel() {

}

void UiPanel::Active() {
	active = !active;
}

bool UiPanel::IsActive()const {
	return active;
}