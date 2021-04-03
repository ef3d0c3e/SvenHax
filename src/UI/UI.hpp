#ifndef UI_UI_HPP
#define UI_UI_HPP

#include "../ImGui/imgui.h"

namespace UI
{
	extern bool isVisible;
	extern ImFont* plex;
	extern ImFont* plex_mono;
	extern ImFont* title_font;

	void SetVisible(bool visible);
	void UpdateColors();
	
	void Draw();
}

#endif // UI_UI_HPP
