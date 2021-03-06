#ifndef UI_WIDGETS_HPP
#define UI_WIDGETS_HPP

#define IMGUI_DEFINE_MATH_OPERATORS
#include "../ImGui/imgui.h"
#include "../ImGui/imgui_internal.h"
#include "../Config.hpp"
#include "Style.hpp"
#include <vector>
#include <string>

namespace UI
{
	void Shadow(const ImVec2& p_min, const ImVec2& p_max, const ImU32 color[4]);
	void RenderArrow(const ImVec2& p_min, ImGuiDir dir, float scale, const ImU32 color);
	void Desc(const char* label);
	void Separator();

	bool Button(const char* label, const ImVec2& size_args = ImVec2(0, 0));
	bool SelectButton(const char* label, bool selected, const ImVec2& size_args = ImVec2(0, 0));
	bool TabList(const char* label, u32 tab, u32 cur, u32 max, ImRect& bb_out, const ImVec2& size_args = ImVec2(0, 0));

	bool SliderFloat(const char* label, float* v, float v_origin, float v_min, float v_max, const char* display_format = "%.3f", float power = 1.f);

	bool Checkbox(const char* label, bool* v);

	bool InputText(const char* label, char* buf, size_t buf_size, ImVec2 size_args = ImVec2(0, 0), ImGuiInputTextFlags flags = 0, ImGuiTextEditCallback callback = NULL, void* user_data = NULL);

	bool BeginCombo(const std::string& label, const std::string& preview, const char* tooltip = NULL, ImGuiComboFlags flags = 0);
	void EndCombo();
	bool Selectable(const char* label, bool selected = false, ImGuiSelectableFlags flags = 0, const ImVec2& size_arg = ImVec2(0,0));
	void SetItemDefaultFocus();
	void CheckboxCombo(const std::vector<std::tuple<const char*, const char*, bool&>>& data, std::string& format); // name, desc, value

	bool ColorButton(const char* desc_id, const ImVec4& col, ImGuiColorEditFlags flags = 0, ImVec2 size = ImVec2(0, 0));
	bool ColorPicker4(const char* label, float col[4], ImGuiColorEditFlags, const float* ref_col);
	bool ColorEdit4(const char* label, ImVec4& col, ImGuiColorEditFlags = 0);
}

#endif // UI_WIDGETS_HPP
