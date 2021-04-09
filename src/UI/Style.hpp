#ifndef UI_COLORS_HPP
#define UI_COLORS_HPP

#include "../ImGui/imgui.h"
#include "../Engine/Primitives/Defs.hpp"

namespace Settings::Style
{
	// Window
	static float child_padding = 10;
	static ImU32 window_bg = 0xF0202020;
	static ImU32 window_accent = 0xFFF02020;
	//static ImU32 title = 0xFFFF0000;
	static ImU32 title = 0xFFFFFFFF;
	static ImU32 child_bg = 0xF0101010;
	static ImU32 tablist_bg[4] = {0xFFAF1010, 0xFFFF0000, 0xFFA02020, 0xFFA01010}; // normal, selected, hovered, active
	static ImU32 tablist_accent = 0xFFF0F0F0;
	static ImU32 tablist_line = 0xFFFF0000;
	static ImU32 tablist_border = 0xFFFF0000;
	static float tablist_current_standout = 8.f;
	static ImU32 tablist_shadow = 0x40000000;
	static float tablist_shadow_scale = 0.01f;

	// Tooltip
	static ImVec2 tooltip_padding(6.f, 6.f);
	static float tooltip_rounding = 4.f;
	static float tooltip_border = 0.f;
	static ImU32 tooltip_color[3] = { 0xFFCACACA, 0xD0D0D0D0, 0xAA1D1D1D }; // text, border, background



	// Buttons
	static ImU32 button_text = 0xFF202020;
	static ImU32 button_bg[3] = { 0xFFE4E4E4, 0xFFCCCCCC, 0xFFAAAAAA }; // normal, hovered, active

	static ImU32 selector_bg[4] = {0x00000000, 0xFF2C2C2C, 0xFF202020, 0xFF101010}; // normal, selected, hovered, active
	static ImU32 selector_accent = 0xFFDF2020;
	static ImU32 selector_border = 0xFF4E4E4E;
	static ImU32 selector_shadow = 0x40000000;
	static float selector_shadow_scale = 0.05f;

	// Sliders
	static float slider_thickness = 0.25f;
	static float grab_radius = 8.f;
	static float slider_value_offset = 10.f;
	static ImU32 slider_grab = 0xFFB5B5B5;
	static ImU32 slider_filled = 0xFF979797;
	static ImU32 slider_empty = 0xFF353535;

	// Checkbox
	static ImU32 checkmark = 0xFFE7E7E7;
	static ImU32 checkbox_bg[4] = { 0xFF383838, 0xFF383838, 0xFF484848, 0xFF282828 }; // normal, checked, hovered, active
	static ImU32 checkbox_border = 0xFF4E4E4E;

	// InputLine
	static ImU32 textinput_bg = 0xFFD0D0D0;
	static ImU32 textinput_text = 0xFF202020;

	// Combo
	static ImU32 arrow_foreground[3] = {0xFF707070, 0xFF707070, 0xFF707070}; // normal, hovered, open
	static ImU32 arrow_background[3] = {0xFFB8B8B8, 0xFFB8B8B8, 0xFFDADADA}; // normal, hovered, open
	static ImU32 combo_text = 0xFF050505;
	static ImU32 combo_background[3] = {0xFFB8B8B8, 0xFFB8B8B8, 0xFFDADADA}; // normal, hovered, open
	static ImU32 combo_popup[5] = {0xFF222222, 0xFF17181A, 0xFF1A1A1A, 0xFF272A2D, 0xFF272A2D}; // border, background, current, hovered, active
	static float combo_text_padding = 6.f;
	static float combo_vertical_padding = 4.f;

	// Colors
	static float color_button_width = 1.80f; // as compared to height
	static ImU32 color_button_border = 0x50FFFFFF;
}

#endif // UI_COLORS_HPP
