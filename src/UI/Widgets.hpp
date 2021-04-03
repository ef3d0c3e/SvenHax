#ifndef UI_WIDGETS_HPP
#define UI_WIDGETS_HPP

#include "../ImGui/imgui.h"
#include "Style.hpp"

namespace UI
{
	bool Button(const char* label, const ImVec2& size_args = ImVec2(0, 0));
	bool SelectButton(const char* label, bool selected, const ImVec2& size_args = ImVec2(0, 0));
	bool TabList(const char* label, u32 tab, u32 cur, u32 max, const ImVec2& size_args = ImVec2(0, 0));

	bool SliderFloat(const char* label, float* v, float v_min, float v_max, const char* display_format = "%.3f", float power = 1.f);

	bool Checkbox(const char* label, bool* v);
}

#endif // UI_WIDGETS_HPP
