#ifndef UI_COLORS_HPP
#define UI_COLORS_HPP

#include "../ImGui/imgui.h"
#include "../Engine/Primitives/Defs.hpp"

namespace Settings
{
namespace Style
{

	static float child_padding = 10;
	static ImU32 window_bg = 0xFF202020;
	static ImU32 window_accent = 0xFFF02020;
	static ImU32 title = 0xFFFF0000;
	static ImU32 child_bg = 0xFF101010;
	static ImU32 tablist_bg[4] = {0xFFAF1010, 0xFFFF2C2C, 0xFFA02020, 0xFFA01010}; // normal, selected, hovered, active
	static ImU32 tablist_accent = 0xFFF0F0F0;
	static ImU32 tablist_line = 0xFFFF0000;
	static ImU32 tablist_border = 0xFFFF0000;

	// Buttons
	static ImU32 button_text = 0xFF202020;
	static ImU32 button_bg[3] = { 0xFFE4E4E4, 0xFFCCCCCC, 0xFFAAAAAA }; // normal, hovered, active

	static ImU32 selector_bg[4] = {0x00000000, 0xFF2C2C2C, 0xFF202020, 0xFF101010}; // normal, selected, hovered, active
	static ImU32 selector_accent = 0xFFDF2020;
	static ImU32 selector_border = 0xFF4E4E4E;

	// Sliders
	static float slider_thickness = 0.3f;
	static float grab_radius = 8.0;
	static float slider_value_offset = 10.f;
	static ImU32 slider_grab = 0xFFB5B5B5;
	static ImU32 slider_filled = 0xFF979797;
	static ImU32 slider_empty = 0xFF353535;

	// Checkbox
	static ImU32 checkmark = 0xFFE7E7E7;
	static ImU32 checkbox_bg[4] = { 0xFF383838, 0xFF17892B, 0xFF484848, 0xFF282828 }; // normal, checked, hovered, active
	static ImU32 checkbox_border = 0xFF4E4E4E;

}
}

#endif // UI_COLORS_HPP
