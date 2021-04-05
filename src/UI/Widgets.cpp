#include "Widgets.hpp"
#define IMGUI_DEFINE_MATH_OPERATORS
#include "../ImGui/imgui.cpp"
#include "../ImGui/imgui_internal.h"
#include <GL/gl.h>

// {{{ Misc
void RenderFrame(ImVec2 p_min, ImVec2 p_max, ImU32 fill_col, float border_size, float rounding, ImU32 border_col, ImDrawCornerFlags corner_flags = ImDrawCornerFlags_All)
{
	ImGuiContext& g = *GImGui;
	ImGuiWindow* window = g.CurrentWindow;
	window->DrawList->AddRectFilled(p_min, p_max, fill_col, rounding);
	if (border_size > 0.0f)
		window->DrawList->AddRect(p_min, p_max, border_col, rounding, corner_flags, border_size);
}

void UI::Shadow(const ImVec2& p_min, const ImVec2& p_max, const ImU32 color[4])
{
	ImGuiContext& g = *GImGui;
	ImGuiWindow* window = g.CurrentWindow;

	window->DrawList->AddRectFilledMultiColor(p_min, p_max, color[0], color[1], color[2], color[3]);
}
// }}}

// {{{ Buttons
bool UI::Button(const char* label, const ImVec2& size_arg)
{
	ImGuiButtonFlags flags = 0;

	ImGuiWindow* window = ImGui::GetCurrentWindow();
	if (window->SkipItems)
		return false;

	ImGuiContext& g = *GImGui;
	const ImGuiStyle& style = g.Style;
	const ImGuiID id = window->GetID(label);
	const ImVec2 label_size = ImGui::CalcTextSize(label, NULL, true);

	ImVec2 pos = window->DC.CursorPos;
	if ((flags & ImGuiButtonFlags_AlignTextBaseLine) && style.FramePadding.y < window->DC.CurrentLineTextBaseOffset) // Try to vertically align buttons that are smaller/have no padding so that text baseline matches (bit hacky, since it shouldn't be a flag)
		pos.y += window->DC.CurrentLineTextBaseOffset - style.FramePadding.y;
	ImVec2 size = ImGui::CalcItemSize(size_arg, label_size.x + style.FramePadding.x * 2.0f + label_size.y * 6, label_size.y + style.FramePadding.y * 2.0f);

	const ImRect bb(pos, pos + size);
	ImGui::ItemSize(bb, style.FramePadding.y);
	if (!ImGui::ItemAdd(bb, id))
		return false;

	if (window->DC.ItemFlags & ImGuiItemFlags_ButtonRepeat)
		flags |= ImGuiButtonFlags_Repeat;
	bool hovered, held;
	bool pressed = ImGui::ButtonBehavior(bb, id, &hovered, &held, flags);
	if (pressed)
		ImGui::MarkItemValueChanged(id);

	// Render
	const ImU32 col = (held && hovered) ? Settings::Style::button_bg[2] : hovered ? Settings::Style::button_bg[1] :
																					  Settings::Style::button_bg[0];
	ImGui::RenderNavHighlight(bb, id);
	ImGui::RenderFrame(bb.Min, bb.Max, col, true, style.FrameRounding);
	ImGui::PushStyleColor(ImGuiCol_Text, Settings::Style::button_text);
	ImGui::RenderTextClipped(bb.Min + style.FramePadding, bb.Max - style.FramePadding, label, NULL, &label_size, style.ButtonTextAlign, &bb);
	ImGui::PopStyleColor();

	return pressed;
}

bool UI::SelectButton(const char* label, bool selected, const ImVec2& size_arg)
{
	ImGuiButtonFlags flags = 0;

	ImGuiWindow* window = ImGui::GetCurrentWindow();
	if (window->SkipItems)
		return false;

	ImGuiContext& g = *GImGui;
	const ImGuiStyle& style = g.Style;
	const ImGuiID id = window->GetID(label);
	const ImVec2 label_size = ImGui::CalcTextSize(label, NULL, true);

	ImVec2 pos = window->DC.CursorPos;
	if ((flags & ImGuiButtonFlags_AlignTextBaseLine) && style.FramePadding.y < window->DC.CurrentLineTextBaseOffset) // Try to vertically align buttons that are smaller/have no padding so that text baseline matches (bit hacky, since it shouldn't be a flag)
		pos.y += window->DC.CurrentLineTextBaseOffset - style.FramePadding.y;
	ImVec2 size = ImGui::CalcItemSize(size_arg, label_size.x + style.FramePadding.x * 2.0f, label_size.y + style.FramePadding.y * 2.0f);

	const ImRect bb(pos, pos + size);
	ImGui::ItemSize(bb, style.FramePadding.y);
	if (!ImGui::ItemAdd(bb, id))
		return false;

	if (window->DC.ItemFlags & ImGuiItemFlags_ButtonRepeat)
		flags |= ImGuiButtonFlags_Repeat;
	bool hovered, held;
	bool pressed = ImGui::ButtonBehavior(bb, id, &hovered, &held, flags);
	if (pressed)
		ImGui::MarkItemValueChanged(id);

	// Render
	const ImU32 col = (held && hovered) ? Settings::Style::selector_bg[3] : hovered ? Settings::Style::selector_bg[2] :
																		selected	? Settings::Style::selector_bg[1] :
																						Settings::Style::selector_bg[0];

	ImGui::RenderNavHighlight(bb, id);
	if (selected)
	{
		RenderFrame(bb.Min, bb.Max, col, 1.f, 0, Settings::Style::selector_border);
		window->DrawList->AddRectFilled(bb.Min, ImVec2(bb.Min.x + label_size.y * .3, bb.Max.y), Settings::Style::selector_accent, 0);
		ImGui::PushStyleColor(ImGuiCol_Text, Settings::Style::selector_accent);
		ImGui::RenderTextClipped(bb.Min + style.FramePadding, bb.Max - style.FramePadding, label, NULL, &label_size, style.ButtonTextAlign, &bb);
		ImGui::PopStyleColor();
	}
	else
	{
		RenderFrame(bb.Min, bb.Max, col, 0.f, 0, Settings::Style::selector_border);
		ImGui::RenderTextClipped(bb.Min + style.FramePadding, bb.Max - style.FramePadding, label, NULL, &label_size, style.ButtonTextAlign, &bb);
	}

	return pressed;
}

bool UI::TabList(const char* label, u32 tab, u32 cur, u32 max, ImRect& bb_out, const ImVec2& size_arg)
{
	ImGuiButtonFlags flags = 0;

	ImGuiWindow* window = ImGui::GetCurrentWindow();
	if (window->SkipItems)
		return false;

	ImGuiContext& g = *GImGui;
	const ImGuiStyle& style = g.Style;
	const ImGuiID id = window->GetID(label);
	const ImVec2 label_size = ImGui::CalcTextSize(label, NULL, true);

	ImVec2 pos = window->DC.CursorPos;
	if ((flags & ImGuiButtonFlags_AlignTextBaseLine) && style.FramePadding.y < window->DC.CurrentLineTextBaseOffset) // Try to vertically align buttons that are smaller/have no padding so that text baseline matches (bit hacky, since it shouldn't be a flag)
		pos.y += window->DC.CurrentLineTextBaseOffset - style.FramePadding.y;
	ImVec2 size = ImGui::CalcItemSize(size_arg, label_size.x + style.FramePadding.x * 2.0f, label_size.y + style.FramePadding.y * 2.0f);

	ImRect bb(pos, pos + size);
	ImGui::ItemSize(bb, style.FramePadding.y);
	if (!ImGui::ItemAdd(bb, id))
		return false;

	if (window->DC.ItemFlags & ImGuiItemFlags_ButtonRepeat)
		flags |= ImGuiButtonFlags_Repeat;
	bool hovered, held;
	bool pressed = ImGui::ButtonBehavior(bb, id, &hovered, &held, flags);
	if (pressed)
		ImGui::MarkItemValueChanged(id);

	// Render
	const ImU32 col = cur == tab ? Settings::Style::tablist_bg[1] : (held && hovered) ? Settings::Style::tablist_bg[3] :
																hovered				  ? Settings::Style::tablist_bg[2] :
																						  Settings::Style::tablist_bg[0];

	ImGui::RenderNavHighlight(bb, id);
	if (cur == tab)
	{
		bb_out = bb;
		bb = ImRect{ bb.Min - ImVec2(0, Settings::Style::tablist_current_standout), bb.Max + ImVec2(0, Settings::Style::tablist_current_standout) };
		window->DrawList->PathLineTo(ImVec2(bb.Min.x, bb.Max.y));
		window->DrawList->PathArcToFast(ImVec2(bb.Min.x + 10.f, bb.Min.y + 10.f), 10.f, 6, 9); //10.f -> rounding
		window->DrawList->PathArcToFast(ImVec2(bb.Max.x - 10.f, bb.Min.y + 10.f), 10.f, 9, 12);
		window->DrawList->PathLineTo(ImVec2(bb.Max.x, bb.Max.y));
		window->DrawList->AddConvexPolyFilled(window->DrawList->_Path.Data, window->DrawList->_Path.Size, col);
		if (g.Style.FrameBorderSize > 0.0f)
			window->DrawList->AddPolyline(window->DrawList->_Path.Data, window->DrawList->_Path.Size, ImGui::GetColorU32(ImGuiCol_Border), false, g.Style.FrameBorderSize);
		window->DrawList->PathClear();

		ImGui::PushStyleColor(ImGuiCol_Text, Settings::Style::tablist_accent);
		ImGui::RenderTextClipped(bb.Min + style.FramePadding, bb.Max - style.FramePadding, label, NULL, &label_size, style.ButtonTextAlign, &bb);
		ImGui::PopStyleColor();
	}
	else
	{
		RenderFrame(bb.Min, bb.Max, col, 0.f, 0.f, Settings::Style::tablist_border, ImDrawCornerFlags_Top | ImDrawCornerFlags_Right);
		ImGui::RenderTextClipped(bb.Min + style.FramePadding, bb.Max - style.FramePadding, label, NULL, &label_size, style.ButtonTextAlign, &bb);
	}

	return pressed;
}
// }}}

// {{{ Sliders
template <typename TYPE, typename SIGNEDTYPE, typename FLOATTYPE>
static bool SliderBehaviorT(const ImRect& bb, ImGuiID id, ImGuiDataType data_type, TYPE* v, const TYPE v_min, const TYPE v_max, const char* format, float power, ImGuiSliderFlags flags)
{
	ImGuiContext& g = *GImGui;
	ImGuiWindow* window = g.CurrentWindow;
	const ImGuiStyle& style = g.Style;

	const bool is_horizontal = (flags & ImGuiSliderFlags_Vertical) == 0;
	const bool is_decimal = (data_type == ImGuiDataType_Float) || (data_type == ImGuiDataType_Double);
	const bool is_power = (power != 1.0f) && is_decimal;

	const float grab_padding = 2.0f;
	const float slider_sz = is_horizontal ? (bb.GetWidth() - grab_padding * 2.0f) : (bb.GetHeight() - grab_padding * 2.0f);
	float grab_sz = style.GrabMinSize;
	SIGNEDTYPE v_range = (v_min < v_max ? v_max - v_min : v_min - v_max);
	if (!is_decimal && v_range >= 0) // v_range < 0 may happen on integer overflows
		grab_sz = ImMax((float)(slider_sz / (v_range + 1)), style.GrabMinSize); // For integer sliders: if possible have the grab size represent 1 unit
	grab_sz = ImMin(grab_sz, slider_sz);
	const float slider_usable_sz = slider_sz - grab_sz;
	const float slider_usable_pos_min = (is_horizontal ? bb.Min.x : bb.Min.y) + grab_padding + grab_sz * 0.5f;
	const float slider_usable_pos_max = (is_horizontal ? bb.Max.x : bb.Max.y) - grab_padding - grab_sz * 0.5f;

	// For power curve sliders that cross over sign boundary we want the curve to be symmetric around 0.0f
	float linear_zero_pos; // 0.0->1.0f
	if (is_power && v_min * v_max < 0.0f)
	{
		// Different sign
		const FLOATTYPE linear_dist_min_to_0 = ImPow(v_min >= 0 ? (FLOATTYPE)v_min : -(FLOATTYPE)v_min, (FLOATTYPE)1.0f / power);
		const FLOATTYPE linear_dist_max_to_0 = ImPow(v_max >= 0 ? (FLOATTYPE)v_max : -(FLOATTYPE)v_max, (FLOATTYPE)1.0f / power);
		linear_zero_pos = (float)(linear_dist_min_to_0 / (linear_dist_min_to_0 + linear_dist_max_to_0));
	}
	else
	{
		// Same sign
		linear_zero_pos = v_min < 0.0f ? 1.0f : 0.0f;
	}

	// Process interacting with the slider
	bool value_changed = false;
	if (g.ActiveId == id)
	{
		bool set_new_value = false;
		float clicked_t = 0.0f;
		if (g.ActiveIdSource == ImGuiInputSource_Mouse)
		{
			if (!g.IO.MouseDown[0])
			{
				ImGui::ClearActiveID();
			}
			else
			{
				const float mouse_abs_pos = is_horizontal ? g.IO.MousePos.x : g.IO.MousePos.y;
				clicked_t = (slider_usable_sz > 0.0f) ? ImClamp((mouse_abs_pos - slider_usable_pos_min) / slider_usable_sz, 0.0f, 1.0f) : 0.0f;
				if (!is_horizontal)
					clicked_t = 1.0f - clicked_t;
				set_new_value = true;
			}
		}
		else if (g.ActiveIdSource == ImGuiInputSource_Nav)
		{
			const ImVec2 delta2 = ImGui::GetNavInputAmount2d(ImGuiNavDirSourceFlags_Keyboard | ImGuiNavDirSourceFlags_PadDPad, ImGuiInputReadMode_RepeatFast, 0.0f, 0.0f);
			float delta = is_horizontal ? delta2.x : -delta2.y;
			if (g.NavActivatePressedId == id && !g.ActiveIdIsJustActivated)
			{
				ImGui::ClearActiveID();
			}
			else if (delta != 0.0f)
			{
				clicked_t = SliderBehaviorCalcRatioFromValue<TYPE, FLOATTYPE>(data_type, *v, v_min, v_max, power, linear_zero_pos);
				const int decimal_precision = is_decimal ? ImParseFormatPrecision(format, 3) : 0;
				if ((decimal_precision > 0) || is_power)
				{
					delta /= 100.0f; // Gamepad/keyboard tweak speeds in % of slider bounds
					if (IsNavInputDown(ImGuiNavInput_TweakSlow))
						delta /= 10.0f;
				}
				else
				{
					if ((v_range >= -100.0f && v_range <= 100.0f) || IsNavInputDown(ImGuiNavInput_TweakSlow))
						delta = ((delta < 0.0f) ? -1.0f : +1.0f) / (float)v_range; // Gamepad/keyboard tweak speeds in integer steps
					else
						delta /= 100.0f;
				}
				if (IsNavInputDown(ImGuiNavInput_TweakFast))
					delta *= 10.0f;
				set_new_value = true;
				if ((clicked_t >= 1.0f && delta > 0.0f) || (clicked_t <= 0.0f && delta < 0.0f)) // This is to avoid applying the saturation when already past the limits
					set_new_value = false;
				else
					clicked_t = ImSaturate(clicked_t + delta);
			}
		}

		if (set_new_value)
		{
			TYPE v_new;
			if (is_power)
			{
				// Account for power curve scale on both sides of the zero
				if (clicked_t < linear_zero_pos)
				{
					// Negative: rescale to the negative range before powering
					float a = 1.0f - (clicked_t / linear_zero_pos);
					a = ImPow(a, power);
					v_new = ImLerp(ImMin(v_max, (TYPE)0), v_min, a);
				}
				else
				{
					// Positive: rescale to the positive range before powering
					float a;
					if (ImFabs(linear_zero_pos - 1.0f) > 1.e-6f)
						a = (clicked_t - linear_zero_pos) / (1.0f - linear_zero_pos);
					else
						a = clicked_t;
					a = ImPow(a, power);
					v_new = ImLerp(ImMax(v_min, (TYPE)0), v_max, a);
				}
			}
			else
			{
				// Linear slider
				if (is_decimal)
				{
					v_new = ImLerp(v_min, v_max, clicked_t);
				}
				else
				{
					// For integer values we want the clicking position to match the grab box so we round above
					// This code is carefully tuned to work with large values (e.g. high ranges of U64) while preserving this property..
					FLOATTYPE v_new_off_f = (v_max - v_min) * clicked_t;
					TYPE v_new_off_floor = (TYPE)(v_new_off_f);
					TYPE v_new_off_round = (TYPE)(v_new_off_f + (FLOATTYPE)0.5);
					if (!is_decimal && v_new_off_floor < v_new_off_round)
						v_new = v_min + v_new_off_round;
					else
						v_new = v_min + v_new_off_floor;
				}
			}

			// Round to user desired precision based on format string
			v_new = RoundScalarWithFormat<TYPE, SIGNEDTYPE>(format, data_type, v_new);

			// Apply result
			if (*v != v_new)
			{
				*v = v_new;
				value_changed = true;
			}
		}
	}

	// Draw
	float grab_t = SliderBehaviorCalcRatioFromValue<TYPE, FLOATTYPE>(data_type, *v, v_min, v_max, power, linear_zero_pos);
	if (!is_horizontal)
		grab_t = 1.0f - grab_t;
	const float grab_pos = ImLerp(slider_usable_pos_min, slider_usable_pos_max, grab_t);
	ImRect grab_bb;
	if (is_horizontal)
		grab_bb = ImRect(grab_pos - grab_sz * 0.5f, bb.Min.y + grab_padding, grab_pos + grab_sz * 0.5f, bb.Max.y - grab_padding);
	else
		grab_bb = ImRect(bb.Min.x + grab_padding, grab_pos - grab_sz * 0.5f, bb.Max.x - grab_padding, grab_pos + grab_sz * 0.5f);


	ImRect rect_bb = bb;
	if (Settings::Style::slider_thickness != 1.0f)
	{
		float shrink_amount = (float)(int)((rect_bb.Max.y - rect_bb.Min.y) * 0.5f * (1.0f - Settings::Style::slider_thickness));
		//bb.Min.y += shrink_amount;
		rect_bb.Max.y -= 2 * shrink_amount;
		grab_bb.Max.y -= 2 * shrink_amount;
	}
	window->DrawList->AddRectFilled(rect_bb.Min, ImVec2(grab_bb.Min.x + (grab_bb.Max.x - grab_bb.Min.x) * 0.65f, rect_bb.Max.y), Settings::Style::slider_filled, style.FrameRounding, ImDrawCornerFlags_Left);
	window->DrawList->AddRectFilled(ImVec2(grab_bb.Max.x - (grab_bb.Max.x - grab_bb.Min.x) * 0.35f, rect_bb.Min.y), rect_bb.Max, Settings::Style::slider_empty, style.FrameRounding, ImDrawCornerFlags_Right);

	window->DrawList->AddCircleFilled((grab_bb.Min + grab_bb.Max) * .5, Settings::Style::grab_radius, Settings::Style::slider_grab);

	// Display value using user-provided display format so user can add prefix/suffix/decorations to the value.
	char value_buf[64];
	const char* value_buf_end = value_buf + DataTypeFormatString(value_buf, IM_ARRAYSIZE(value_buf), data_type, v, format);
	ImGui::RenderTextClipped(bb.Min + ImVec2(0, Settings::Style::slider_value_offset), bb.Max, value_buf, value_buf_end, NULL, ImVec2(0.5f, 0.5f));

	return value_changed;
}

static bool SliderBehavior(const ImRect& frame_bb, ImGuiID id, ImGuiDataType data_type, void* v, const void* v_min, const void* v_max, const char* format, float power, ImGuiSliderFlags flags = 0)
{
	// Draw frame
	ImGuiContext& g = *GImGui;
	ImGui::RenderNavHighlight(frame_bb, id);

	switch (data_type)
	{
		case ImGuiDataType_S32:
			IM_ASSERT(*(const ImS32*)v_min >= IM_S32_MIN / 2 && *(const ImS32*)v_max <= IM_S32_MAX / 2);
			return SliderBehaviorT<ImS32, ImS32, float>(frame_bb, id, data_type, (ImS32*)v, *(const ImS32*)v_min, *(const ImS32*)v_max, format, power, flags);
		case ImGuiDataType_U32:
			IM_ASSERT(*(const ImU32*)v_min <= IM_U32_MAX / 2);
			return SliderBehaviorT<ImU32, ImS32, float>(frame_bb, id, data_type, (ImU32*)v, *(const ImU32*)v_min, *(const ImU32*)v_max, format, power, flags);
		case ImGuiDataType_S64:
			IM_ASSERT(*(const ImS64*)v_min >= IM_S64_MIN / 2 && *(const ImS64*)v_max <= IM_S64_MAX / 2);
			return SliderBehaviorT<ImS64, ImS64, double>(frame_bb, id, data_type, (ImS64*)v, *(const ImS64*)v_min, *(const ImS64*)v_max, format, power, flags);
		case ImGuiDataType_U64:
			IM_ASSERT(*(const ImU64*)v_min <= IM_U64_MAX / 2);
			return SliderBehaviorT<ImU64, ImS64, double>(frame_bb, id, data_type, (ImU64*)v, *(const ImU64*)v_min, *(const ImU64*)v_max, format, power, flags);
		case ImGuiDataType_Float:
			IM_ASSERT(*(const float*)v_min >= -FLT_MAX / 2.0f && *(const float*)v_max <= FLT_MAX / 2.0f);
			return SliderBehaviorT<float, float, float>(frame_bb, id, data_type, (float*)v, *(const float*)v_min, *(const float*)v_max, format, power, flags);
		case ImGuiDataType_Double:
			IM_ASSERT(*(const double*)v_min >= -DBL_MAX / 2.0f && *(const double*)v_max <= DBL_MAX / 2.0f);
			return SliderBehaviorT<double, double, double>(frame_bb, id, data_type, (double*)v, *(const double*)v_min, *(const double*)v_max, format, power, flags);
		case ImGuiDataType_COUNT:
			break;
	}
	IM_ASSERT(0);
	return false;
}

static bool SliderScalar(const char* label, ImGuiDataType data_type, void* v, const void* v_min, const void* v_max, const char* format = NULL, float power = 1.f)
{
	ImGuiWindow* window = ImGui::GetCurrentWindow();
	if (window->SkipItems)
		return false;

	ImGuiContext& g = *GImGui;
	const ImGuiStyle& style = g.Style;
	const ImGuiID id = window->GetID(label);
	const ImVec2 label_size = ImGui::CalcTextSize(label, NULL, true);
	const float w = (label_size.x == 0) ? ImGui::GetContentRegionAvail().x : ImGui::CalcItemWidth();

	const ImRect frame_bb(window->DC.CursorPos + ImVec2(.5f * Settings::Style::grab_radius, 0), window->DC.CursorPos + ImVec2(w - Settings::Style::grab_radius, label_size.y + style.FramePadding.y * 2.0f));
	const ImRect total_bb(frame_bb.Min, frame_bb.Max + ImVec2(label_size.x > 0.0f ? style.ItemInnerSpacing.x + label_size.x : 0.0f, 0.0f));

	// NB- we don't call ItemSize() yet because we may turn into a text edit box below
	if (!ImGui::ItemAdd(total_bb, id, &frame_bb))
	{
		ImGui::ItemSize(total_bb, style.FramePadding.y);
		return false;
	}

	// Default format string when passing NULL
	// Patch old "%.0f" format string to use "%d", read function comments for more details.
	IM_ASSERT(data_type >= 0 && data_type < ImGuiDataType_COUNT);
	if (format == NULL)
		format = GDataTypeInfo[data_type].PrintFmt;
	else if (data_type == ImGuiDataType_S32 && strcmp(format, "%d") != 0)
		format = PatchFormatStringFloatToInt(format);

	// Tabbing or CTRL-clicking on Slider turns it into an input box
	bool start_text_input = false;
	const bool tab_focus_requested = ImGui::FocusableItemRegister(window, id);
	const bool hovered = ImGui::ItemHoverable(frame_bb, id);
	if (tab_focus_requested || (hovered && g.IO.MouseClicked[0]) || g.NavActivateId == id || (g.NavInputId == id && g.ScalarAsInputTextId != id))
	{
		ImGui::SetActiveID(id, window);
		ImGui::SetFocusID(id, window);
		ImGui::FocusWindow(window);
		g.ActiveIdAllowNavDirFlags = (1 << ImGuiDir_Up) | (1 << ImGuiDir_Down);
		if (tab_focus_requested || g.IO.KeyCtrl || g.NavInputId == id)
		{
			start_text_input = true;
			g.ScalarAsInputTextId = 0;
		}
	}
	if (start_text_input || (g.ActiveId == id && g.ScalarAsInputTextId == id))
		return ImGui::InputScalarAsWidgetReplacement(frame_bb, id, label, data_type, v, format);

	// Actual slider behavior + render grab
	ImGui::ItemSize(total_bb, style.FramePadding.y);
	const bool value_changed = SliderBehavior(frame_bb, id, data_type, v, v_min, v_max, format, power);
	if (value_changed)
		ImGui::MarkItemValueChanged(id);

	if (label_size.x > 0.0f)
		ImGui::RenderText(ImVec2(frame_bb.Max.x + style.ItemInnerSpacing.x, frame_bb.Min.y + style.FramePadding.y), label);

	return value_changed;
}

bool UI::SliderFloat(const char* label, float* v, float v_min, float v_max, const char* display_format, float power)
{
	return SliderScalar(label, ImGuiDataType_Float, v, &v_min, &v_max, display_format, power);
}
// }}}

// {{{ Checkbox
bool UI::Checkbox(const char* label, bool* v)
{
	ImGuiWindow* window = ImGui::GetCurrentWindow();
	if (window->SkipItems)
		return false;

	ImGuiContext& g = *GImGui;
	const ImGuiStyle& style = g.Style;
	const ImGuiID id = window->GetID(label);
	const ImVec2 label_size = ImGui::CalcTextSize(label, NULL, true);

	const ImRect check_bb(window->DC.CursorPos + ImVec2(.25 * style.FramePadding.x, .25 * style.FramePadding.y), window->DC.CursorPos + ImVec2(label_size.y + 1.5 * style.FramePadding.x, label_size.y + 1.5 * style.FramePadding.y)); // We want a square shape to we use Y twice
	ImGui::ItemSize(check_bb, style.FramePadding.y);

	ImRect total_bb = check_bb;
	if (label_size.x > 0)
		ImGui::SameLine(0, style.ItemInnerSpacing.x);
	const ImRect text_bb(window->DC.CursorPos + ImVec2(style.FramePadding.y, style.FramePadding.y), window->DC.CursorPos + ImVec2(style.FramePadding.y, style.FramePadding.y) + label_size);
	if (label_size.x > 0)
	{
		ImGui::ItemSize(ImVec2(text_bb.GetWidth(), check_bb.GetHeight()), style.FramePadding.y);
		total_bb = ImRect(ImMin(check_bb.Min, text_bb.Min), ImMax(check_bb.Max, text_bb.Max));
	}

	if (!ImGui::ItemAdd(total_bb, id))
		return false;

	bool hovered, held;
	bool pressed = ImGui::ButtonBehavior(total_bb, id, &hovered, &held);
	if (pressed)
	{
		*v = !(*v);
		ImGui::MarkItemValueChanged(id);
	}

	ImGui::RenderNavHighlight(total_bb, id);
	RenderFrame(check_bb.Min, check_bb.Max, (held && hovered) ? Settings::Style::checkbox_bg[3] : hovered ? Settings::Style::checkbox_bg[2] :
																							  *v		  ? Settings::Style::checkbox_bg[1] :
																											  Settings::Style::checkbox_bg[0],
				1.f, 4.f, Settings::Style::checkbox_border);
	if (*v)
	{
		const float check_sz = ImMin(check_bb.GetWidth(), check_bb.GetHeight());
		const float pad = ImMax(1.0f, (float)(int)(check_sz / 6.0f));
		ImGui::RenderCheckMark(check_bb.Min + ImVec2(pad, pad), Settings::Style::checkmark, check_bb.GetWidth() - pad * 2.0f);
	}

	if (label_size.x > 0.0f)
		ImGui::RenderText(text_bb.Min, label);

	return pressed;
}
// }}}

// {{{ InputText
bool UI::InputText(const char* label, char* buf, size_t buf_size, ImVec2 size_arg, ImGuiInputTextFlags flags, ImGuiTextEditCallback callback, void* user_data)
{
	ImGuiWindow* window = ImGui::GetCurrentWindow();
	if (window->SkipItems)
		return false;

	IM_ASSERT(!((flags & ImGuiInputTextFlags_CallbackHistory) && (flags & ImGuiInputTextFlags_Multiline))); // Can't use both together (they both use up/down keys)
	IM_ASSERT(!((flags & ImGuiInputTextFlags_CallbackCompletion) && (flags & ImGuiInputTextFlags_AllowTabInput))); // Can't use both together (they both use tab key)

	ImGuiContext& g = *GImGui;
	const ImGuiIO& io = g.IO;
	const ImGuiStyle& style = g.Style;

	const bool is_multiline = (flags & ImGuiInputTextFlags_Multiline) != 0;
	const bool is_editable = (flags & ImGuiInputTextFlags_ReadOnly) == 0;
	const bool is_password = (flags & ImGuiInputTextFlags_Password) != 0;
	const bool is_undoable = (flags & ImGuiInputTextFlags_NoUndoRedo) == 0;

	if (is_multiline) // Open group before calling GetID() because groups tracks id created within their scope,
		ImGui::BeginGroup();
	const ImGuiID id = window->GetID(label);
	const ImVec2 label_size = ImGui::CalcTextSize(label, NULL, true);
	ImVec2 size = ImGui::CalcItemSize(size_arg, ImGui::CalcItemWidth(), (is_multiline ? ImGui::GetTextLineHeight() * 8.0f : label_size.y) + style.FramePadding.y * 2.0f); // Arbitrary default of 8 lines high for multi-line
	const ImRect frame_bb(window->DC.CursorPos, window->DC.CursorPos + size);
	const ImRect total_bb(frame_bb.Min, frame_bb.Max + ImVec2(label_size.x > 0.0f ? (style.ItemInnerSpacing.x + label_size.x) : 0.0f, 0.0f));

	ImGuiWindow* draw_window = window;
	if (is_multiline)
	{
		ImGui::ItemAdd(total_bb, id, &frame_bb);
		if (!ImGui::BeginChildFrame(id, frame_bb.GetSize()))
		{
			ImGui::EndChildFrame();
			ImGui::EndGroup();
			return false;
		}
		draw_window = ImGui::GetCurrentWindow();
		draw_window->DC.NavLayerActiveMaskNext |= draw_window->DC.NavLayerCurrentMask; // This is to ensure that EndChild() will display a navigation highlight
		size.x -= draw_window->ScrollbarSizes.x;
	}
	else
	{
		ImGui::ItemSize(total_bb, style.FramePadding.y);
		if (!ImGui::ItemAdd(total_bb, id, &frame_bb))
			return false;
	}
	const bool hovered = ImGui::ItemHoverable(frame_bb, id);
	if (hovered)
		g.MouseCursor = ImGuiMouseCursor_TextInput;

	// Password pushes a temporary font with only a fallback glyph
	if (is_password)
	{
		const ImFontGlyph* glyph = g.Font->FindGlyph('*');
		ImFont* password_font = &g.InputTextPasswordFont;
		password_font->FontSize = g.Font->FontSize;
		password_font->Scale = g.Font->Scale;
		password_font->DisplayOffset = g.Font->DisplayOffset;
		password_font->Ascent = g.Font->Ascent;
		password_font->Descent = g.Font->Descent;
		password_font->ContainerAtlas = g.Font->ContainerAtlas;
		password_font->FallbackGlyph = glyph;
		password_font->FallbackAdvanceX = glyph->AdvanceX;
		IM_ASSERT(password_font->Glyphs.empty() && password_font->IndexAdvanceX.empty() && password_font->IndexLookup.empty());
		ImGui::PushFont(password_font);
	}

	// NB: we are only allowed to access 'edit_state' if we are the active widget.
	ImGuiTextEditState& edit_state = g.InputTextState;

	const bool focus_requested = ImGui::FocusableItemRegister(window, id, (flags & (ImGuiInputTextFlags_CallbackCompletion | ImGuiInputTextFlags_AllowTabInput)) == 0); // Using completion callback disable keyboard tabbing
	const bool focus_requested_by_code = focus_requested && (window->FocusIdxAllCounter == window->FocusIdxAllRequestCurrent);
	const bool focus_requested_by_tab = focus_requested && !focus_requested_by_code;

	const bool user_clicked = hovered && io.MouseClicked[0];
	const bool user_scrolled = is_multiline && g.ActiveId == 0 && edit_state.Id == id && g.ActiveIdPreviousFrame == draw_window->GetIDNoKeepAlive("#SCROLLY");
	const bool user_nav_input_start = (g.ActiveId != id) && ((g.NavInputId == id) || (g.NavActivateId == id && g.NavInputSource == ImGuiInputSource_NavKeyboard));

	bool clear_active_id = false;

	bool select_all = (g.ActiveId != id) && ((flags & ImGuiInputTextFlags_AutoSelectAll) != 0 || user_nav_input_start) && (!is_multiline);
	if (focus_requested || user_clicked || user_scrolled || user_nav_input_start)
	{
		if (g.ActiveId != id)
		{
			// Start edition
			// Take a copy of the initial buffer value (both in original UTF-8 format and converted to wchar)
			// From the moment we focused we are ignoring the content of 'buf' (unless we are in read-only mode)
			const int prev_len_w = edit_state.CurLenW;
			edit_state.Text.resize(buf_size + 1); // wchar count <= UTF-8 count. we use +1 to make sure that .Data isn't NULL so it doesn't crash.
			edit_state.InitialText.resize(buf_size + 1); // UTF-8. we use +1 to make sure that .Data isn't NULL so it doesn't crash.
			ImStrncpy(edit_state.InitialText.Data, buf, edit_state.InitialText.Size);
			const char* buf_end = NULL;
			edit_state.CurLenW = ImTextStrFromUtf8(edit_state.Text.Data, edit_state.Text.Size, buf, NULL, &buf_end);
			edit_state.CurLenA = (int)(buf_end - buf); // We can't get the result from ImFormatString() above because it is not UTF-8 aware. Here we'll cut off malformed UTF-8.
			edit_state.CursorAnimReset();

			// Preserve cursor position and undo/redo stack if we come back to same widget
			// FIXME: We should probably compare the whole buffer to be on the safety side. Comparing buf (utf8) and edit_state.Text (wchar).
			const bool recycle_state = (edit_state.Id == id) && (prev_len_w == edit_state.CurLenW);
			if (recycle_state)
			{
				// Recycle existing cursor/selection/undo stack but clamp position
				// Note a single mouse click will override the cursor/position immediately by calling stb_textedit_click handler.
				edit_state.CursorClamp();
			}
			else
			{
				edit_state.Id = id;
				edit_state.ScrollX = 0.0f;
				stb_textedit_initialize_state(&edit_state.StbState, !is_multiline);
				if (!is_multiline && focus_requested_by_code)
					select_all = true;
			}
			if (flags & ImGuiInputTextFlags_AlwaysInsertMode)
				edit_state.StbState.insert_mode = true;
			if (!is_multiline && (focus_requested_by_tab || (user_clicked && io.KeyCtrl)))
				select_all = true;
		}
		ImGui::SetActiveID(id, window);
		ImGui::SetFocusID(id, window);
		ImGui::FocusWindow(window);
		if (!is_multiline && !(flags & ImGuiInputTextFlags_CallbackHistory))
			g.ActiveIdAllowNavDirFlags |= ((1 << ImGuiDir_Up) | (1 << ImGuiDir_Down));
	}
	else if (io.MouseClicked[0])
	{
		// Release focus when we click outside
		clear_active_id = true;
	}

	bool value_changed = false;
	bool enter_pressed = false;

	if (g.ActiveId == id)
	{
		if (!is_editable && !g.ActiveIdIsJustActivated)
		{
			// When read-only we always use the live data passed to the function
			edit_state.Text.resize(buf_size + 1);
			const char* buf_end = NULL;
			edit_state.CurLenW = ImTextStrFromUtf8(edit_state.Text.Data, edit_state.Text.Size, buf, NULL, &buf_end);
			edit_state.CurLenA = (int)(buf_end - buf);
			edit_state.CursorClamp();
		}

		edit_state.BufSizeA = buf_size;

		// Although we are active we don't prevent mouse from hovering other elements unless we are interacting right now with the widget.
		// Down the line we should have a cleaner library-wide concept of Selected vs Active.
		g.ActiveIdAllowOverlap = !io.MouseDown[0];
		g.WantTextInputNextFrame = 1;

		// Edit in progress
		const float mouse_x = (io.MousePos.x - frame_bb.Min.x - style.FramePadding.x) + edit_state.ScrollX;
		const float mouse_y = (is_multiline ? (io.MousePos.y - draw_window->DC.CursorPos.y - style.FramePadding.y) : (g.FontSize * 0.5f));

		const bool is_osx = io.OptMacOSXBehaviors;
		if (select_all || (hovered && !is_osx && io.MouseDoubleClicked[0]))
		{
			edit_state.SelectAll();
			edit_state.SelectedAllMouseLock = true;
		}
		else if (hovered && is_osx && io.MouseDoubleClicked[0])
		{
			// Double-click select a word only, OS X style (by simulating keystrokes)
			edit_state.OnKeyPressed(STB_TEXTEDIT_K_WORDLEFT);
			edit_state.OnKeyPressed(STB_TEXTEDIT_K_WORDRIGHT | STB_TEXTEDIT_K_SHIFT);
		}
		else if (io.MouseClicked[0] && !edit_state.SelectedAllMouseLock)
		{
			if (hovered)
			{
				stb_textedit_click(&edit_state, &edit_state.StbState, mouse_x, mouse_y);
				edit_state.CursorAnimReset();
			}
		}
		else if (io.MouseDown[0] && !edit_state.SelectedAllMouseLock && (io.MouseDelta.x != 0.0f || io.MouseDelta.y != 0.0f))
		{
			stb_textedit_drag(&edit_state, &edit_state.StbState, mouse_x, mouse_y);
			edit_state.CursorAnimReset();
			edit_state.CursorFollow = true;
		}
		if (edit_state.SelectedAllMouseLock && !io.MouseDown[0])
			edit_state.SelectedAllMouseLock = false;

		if (io.InputCharacters[0])
		{
			// Process text input (before we check for Return because using some IME will effectively send a Return?)
			// We ignore CTRL inputs, but need to allow ALT+CTRL as some keyboards (e.g. German) use AltGR (which _is_ Alt+Ctrl) to input certain characters.
			bool ignore_inputs = (io.KeyCtrl && !io.KeyAlt) || (is_osx && io.KeySuper);
			if (!ignore_inputs && is_editable && !user_nav_input_start)
				for (int n = 0; n < IM_ARRAYSIZE(io.InputCharacters) && io.InputCharacters[n]; n++)
				{
					// Insert character if they pass filtering
					unsigned int c = (unsigned int)io.InputCharacters[n];
					if (InputTextFilterCharacter(&c, flags, callback, user_data))
						edit_state.OnKeyPressed((int)c);
				}

			// Consume characters
			memset(g.IO.InputCharacters, 0, sizeof(g.IO.InputCharacters));
		}
	}

	bool cancel_edit = false;
	if (g.ActiveId == id && !g.ActiveIdIsJustActivated && !clear_active_id)
	{
		// Handle key-presses
		const int k_mask = (io.KeyShift ? STB_TEXTEDIT_K_SHIFT : 0);
		const bool is_osx = io.OptMacOSXBehaviors;
		const bool is_shortcut_key = (is_osx ? (io.KeySuper && !io.KeyCtrl) : (io.KeyCtrl && !io.KeySuper)) && !io.KeyAlt && !io.KeyShift; // OS X style: Shortcuts using Cmd/Super instead of Ctrl
		const bool is_osx_shift_shortcut = is_osx && io.KeySuper && io.KeyShift && !io.KeyCtrl && !io.KeyAlt;
		const bool is_wordmove_key_down = is_osx ? io.KeyAlt : io.KeyCtrl; // OS X style: Text editing cursor movement using Alt instead of Ctrl
		const bool is_startend_key_down = is_osx && io.KeySuper && !io.KeyCtrl && !io.KeyAlt; // OS X style: Line/Text Start and End using Cmd+Arrows instead of Home/End
		const bool is_ctrl_key_only = io.KeyCtrl && !io.KeyShift && !io.KeyAlt && !io.KeySuper;
		const bool is_shift_key_only = io.KeyShift && !io.KeyCtrl && !io.KeyAlt && !io.KeySuper;

		const bool is_cut = ((is_shortcut_key && IsKeyPressedMap(ImGuiKey_X)) || (is_shift_key_only && IsKeyPressedMap(ImGuiKey_Delete))) && is_editable && !is_password && (!is_multiline || edit_state.HasSelection());
		const bool is_copy = ((is_shortcut_key && IsKeyPressedMap(ImGuiKey_C)) || (is_ctrl_key_only && IsKeyPressedMap(ImGuiKey_Insert))) && !is_password && (!is_multiline || edit_state.HasSelection());
		const bool is_paste = ((is_shortcut_key && IsKeyPressedMap(ImGuiKey_V)) || (is_shift_key_only && IsKeyPressedMap(ImGuiKey_Insert))) && is_editable;
		const bool is_undo = ((is_shortcut_key && IsKeyPressedMap(ImGuiKey_Z)) && is_editable && is_undoable);
		const bool is_redo = ((is_shortcut_key && IsKeyPressedMap(ImGuiKey_Y)) || (is_osx_shift_shortcut && IsKeyPressedMap(ImGuiKey_Z))) && is_editable && is_undoable;

		if (IsKeyPressedMap(ImGuiKey_LeftArrow))
		{
			edit_state.OnKeyPressed((is_startend_key_down ? STB_TEXTEDIT_K_LINESTART : is_wordmove_key_down ? STB_TEXTEDIT_K_WORDLEFT :
																												STB_TEXTEDIT_K_LEFT) |
									k_mask);
		}
		else if (IsKeyPressedMap(ImGuiKey_RightArrow))
		{
			edit_state.OnKeyPressed((is_startend_key_down ? STB_TEXTEDIT_K_LINEEND : is_wordmove_key_down ? STB_TEXTEDIT_K_WORDRIGHT :
																											  STB_TEXTEDIT_K_RIGHT) |
									k_mask);
		}
		else if (IsKeyPressedMap(ImGuiKey_UpArrow) && is_multiline)
		{
			if (io.KeyCtrl)
				SetWindowScrollY(draw_window, ImMax(draw_window->Scroll.y - g.FontSize, 0.0f));
			else
				edit_state.OnKeyPressed((is_startend_key_down ? STB_TEXTEDIT_K_TEXTSTART : STB_TEXTEDIT_K_UP) | k_mask);
		}
		else if (IsKeyPressedMap(ImGuiKey_DownArrow) && is_multiline)
		{
			if (io.KeyCtrl)
				SetWindowScrollY(draw_window, ImMin(draw_window->Scroll.y + g.FontSize, ImGui::GetScrollMaxY()));
			else
				edit_state.OnKeyPressed((is_startend_key_down ? STB_TEXTEDIT_K_TEXTEND : STB_TEXTEDIT_K_DOWN) | k_mask);
		}
		else if (IsKeyPressedMap(ImGuiKey_Home))
		{
			edit_state.OnKeyPressed(io.KeyCtrl ? STB_TEXTEDIT_K_TEXTSTART | k_mask : STB_TEXTEDIT_K_LINESTART | k_mask);
		}
		else if (IsKeyPressedMap(ImGuiKey_End))
		{
			edit_state.OnKeyPressed(io.KeyCtrl ? STB_TEXTEDIT_K_TEXTEND | k_mask : STB_TEXTEDIT_K_LINEEND | k_mask);
		}
		else if (IsKeyPressedMap(ImGuiKey_Delete) && is_editable)
		{
			edit_state.OnKeyPressed(STB_TEXTEDIT_K_DELETE | k_mask);
		}
		else if (IsKeyPressedMap(ImGuiKey_Backspace) && is_editable)
		{
			if (!edit_state.HasSelection())
			{
				if (is_wordmove_key_down)
					edit_state.OnKeyPressed(STB_TEXTEDIT_K_WORDLEFT | STB_TEXTEDIT_K_SHIFT);
				else if (is_osx && io.KeySuper && !io.KeyAlt && !io.KeyCtrl)
					edit_state.OnKeyPressed(STB_TEXTEDIT_K_LINESTART | STB_TEXTEDIT_K_SHIFT);
			}
			edit_state.OnKeyPressed(STB_TEXTEDIT_K_BACKSPACE | k_mask);
		}
		else if (IsKeyPressedMap(ImGuiKey_Enter))
		{
			bool ctrl_enter_for_new_line = (flags & ImGuiInputTextFlags_CtrlEnterForNewLine) != 0;
			if (!is_multiline || (ctrl_enter_for_new_line && !io.KeyCtrl) || (!ctrl_enter_for_new_line && io.KeyCtrl))
			{
				enter_pressed = clear_active_id = true;
			}
			else if (is_editable)
			{
				unsigned int c = '\n'; // Insert new line
				if (InputTextFilterCharacter(&c, flags, callback, user_data))
					edit_state.OnKeyPressed((int)c);
			}
		}
		else if ((flags & ImGuiInputTextFlags_AllowTabInput) && IsKeyPressedMap(ImGuiKey_Tab) && !io.KeyCtrl && !io.KeyShift && !io.KeyAlt && is_editable)
		{
			unsigned int c = '\t'; // Insert TAB
			if (InputTextFilterCharacter(&c, flags, callback, user_data))
				edit_state.OnKeyPressed((int)c);
		}
		else if (IsKeyPressedMap(ImGuiKey_Escape))
		{
			clear_active_id = cancel_edit = true;
		}
		else if (is_undo || is_redo)
		{
			edit_state.OnKeyPressed(is_undo ? STB_TEXTEDIT_K_UNDO : STB_TEXTEDIT_K_REDO);
			edit_state.ClearSelection();
		}
		else if (is_shortcut_key && IsKeyPressedMap(ImGuiKey_A))
		{
			edit_state.SelectAll();
			edit_state.CursorFollow = true;
		}
		else if (is_cut || is_copy)
		{
			// Cut, Copy
			if (io.SetClipboardTextFn)
			{
				const int ib = edit_state.HasSelection() ? ImMin(edit_state.StbState.select_start, edit_state.StbState.select_end) : 0;
				const int ie = edit_state.HasSelection() ? ImMax(edit_state.StbState.select_start, edit_state.StbState.select_end) : edit_state.CurLenW;
				edit_state.TempTextBuffer.resize((ie - ib) * 4 + 1);
				ImTextStrToUtf8(edit_state.TempTextBuffer.Data, edit_state.TempTextBuffer.Size, edit_state.Text.Data + ib, edit_state.Text.Data + ie);
				ImGui::SetClipboardText(edit_state.TempTextBuffer.Data);
			}
			if (is_cut)
			{
				if (!edit_state.HasSelection())
					edit_state.SelectAll();
				edit_state.CursorFollow = true;
				stb_textedit_cut(&edit_state, &edit_state.StbState);
			}
		}
		else if (is_paste)
		{
			if (const char* clipboard = ImGui::GetClipboardText())
			{
				// Filter pasted buffer
				const int clipboard_len = (int)strlen(clipboard);
				ImWchar* clipboard_filtered = (ImWchar*)ImGui::MemAlloc((clipboard_len + 1) * sizeof(ImWchar));
				int clipboard_filtered_len = 0;
				for (const char* s = clipboard; *s;)
				{
					unsigned int c;
					s += ImTextCharFromUtf8(&c, s, NULL);
					if (c == 0)
						break;
					if (c >= 0x10000 || !InputTextFilterCharacter(&c, flags, callback, user_data))
						continue;
					clipboard_filtered[clipboard_filtered_len++] = (ImWchar)c;
				}
				clipboard_filtered[clipboard_filtered_len] = 0;
				if (clipboard_filtered_len > 0) // If everything was filtered, ignore the pasting operation
				{
					stb_textedit_paste(&edit_state, &edit_state.StbState, clipboard_filtered, clipboard_filtered_len);
					edit_state.CursorFollow = true;
				}
				ImGui::MemFree(clipboard_filtered);
			}
		}
	}

	if (g.ActiveId == id)
	{
		if (cancel_edit)
		{
			// Restore initial value. Only return true if restoring to the initial value changes the current buffer contents.
			if (is_editable && strncmp(buf, edit_state.InitialText.Data, buf_size) != 0)
			{
				ImStrncpy(buf, edit_state.InitialText.Data, buf_size);
				value_changed = true;
			}
		}

		// When using 'ImGuiInputTextFlags_EnterReturnsTrue' as a special case we reapply the live buffer back to the input buffer before clearing ActiveId, even though strictly speaking it wasn't modified on this frame.
		// If we didn't do that, code like InputInt() with ImGuiInputTextFlags_EnterReturnsTrue would fail. Also this allows the user to use InputText() with ImGuiInputTextFlags_EnterReturnsTrue without maintaining any user-side storage.
		bool apply_edit_back_to_user_buffer = !cancel_edit || (enter_pressed && (flags & ImGuiInputTextFlags_EnterReturnsTrue) != 0);
		if (apply_edit_back_to_user_buffer)
		{
			// Apply new value immediately - copy modified buffer back
			// Note that as soon as the input box is active, the in-widget value gets priority over any underlying modification of the input buffer
			// FIXME: We actually always render 'buf' when calling DrawList->AddText, making the comment above incorrect.
			// FIXME-OPT: CPU waste to do this every time the widget is active, should mark dirty state from the stb_textedit callbacks.
			if (is_editable)
			{
				edit_state.TempTextBuffer.resize(edit_state.Text.Size * 4);
				ImTextStrToUtf8(edit_state.TempTextBuffer.Data, edit_state.TempTextBuffer.Size, edit_state.Text.Data, NULL);
			}

			// User callback
			if ((flags & (ImGuiInputTextFlags_CallbackCompletion | ImGuiInputTextFlags_CallbackHistory | ImGuiInputTextFlags_CallbackAlways)) != 0)
			{
				IM_ASSERT(callback != NULL);

				// The reason we specify the usage semantic (Completion/History) is that Completion needs to disable keyboard TABBING at the moment.
				ImGuiInputTextFlags event_flag = 0;
				ImGuiKey event_key = ImGuiKey_COUNT;
				if ((flags & ImGuiInputTextFlags_CallbackCompletion) != 0 && IsKeyPressedMap(ImGuiKey_Tab))
				{
					event_flag = ImGuiInputTextFlags_CallbackCompletion;
					event_key = ImGuiKey_Tab;
				}
				else if ((flags & ImGuiInputTextFlags_CallbackHistory) != 0 && IsKeyPressedMap(ImGuiKey_UpArrow))
				{
					event_flag = ImGuiInputTextFlags_CallbackHistory;
					event_key = ImGuiKey_UpArrow;
				}
				else if ((flags & ImGuiInputTextFlags_CallbackHistory) != 0 && IsKeyPressedMap(ImGuiKey_DownArrow))
				{
					event_flag = ImGuiInputTextFlags_CallbackHistory;
					event_key = ImGuiKey_DownArrow;
				}
				else if (flags & ImGuiInputTextFlags_CallbackAlways)
					event_flag = ImGuiInputTextFlags_CallbackAlways;

				if (event_flag)
				{
					ImGuiTextEditCallbackData callback_data;
					memset(&callback_data, 0, sizeof(ImGuiTextEditCallbackData));
					callback_data.EventFlag = event_flag;
					callback_data.Flags = flags;
					callback_data.UserData = user_data;
					callback_data.ReadOnly = !is_editable;

					callback_data.EventKey = event_key;
					callback_data.Buf = edit_state.TempTextBuffer.Data;
					callback_data.BufTextLen = edit_state.CurLenA;
					callback_data.BufSize = edit_state.BufSizeA;
					callback_data.BufDirty = false;

					// We have to convert from wchar-positions to UTF-8-positions, which can be pretty slow (an incentive to ditch the ImWchar buffer, see https://github.com/nothings/stb/issues/188)
					ImWchar* text = edit_state.Text.Data;
					const int utf8_cursor_pos = callback_data.CursorPos = ImTextCountUtf8BytesFromStr(text, text + edit_state.StbState.cursor);
					const int utf8_selection_start = callback_data.SelectionStart = ImTextCountUtf8BytesFromStr(text, text + edit_state.StbState.select_start);
					const int utf8_selection_end = callback_data.SelectionEnd = ImTextCountUtf8BytesFromStr(text, text + edit_state.StbState.select_end);

					// Call user code
					callback(&callback_data);

					// Read back what user may have modified
					IM_ASSERT(callback_data.Buf == edit_state.TempTextBuffer.Data); // Invalid to modify those fields
					IM_ASSERT(callback_data.BufSize == edit_state.BufSizeA);
					IM_ASSERT(callback_data.Flags == flags);
					if (callback_data.CursorPos != utf8_cursor_pos)
						edit_state.StbState.cursor = ImTextCountCharsFromUtf8(callback_data.Buf, callback_data.Buf + callback_data.CursorPos);
					if (callback_data.SelectionStart != utf8_selection_start)
						edit_state.StbState.select_start = ImTextCountCharsFromUtf8(callback_data.Buf, callback_data.Buf + callback_data.SelectionStart);
					if (callback_data.SelectionEnd != utf8_selection_end)
						edit_state.StbState.select_end = ImTextCountCharsFromUtf8(callback_data.Buf, callback_data.Buf + callback_data.SelectionEnd);
					if (callback_data.BufDirty)
					{
						IM_ASSERT(callback_data.BufTextLen == (int)strlen(callback_data.Buf)); // You need to maintain BufTextLen if you change the text!
						edit_state.CurLenW = ImTextStrFromUtf8(edit_state.Text.Data, edit_state.Text.Size, callback_data.Buf, NULL);
						edit_state.CurLenA = callback_data.BufTextLen; // Assume correct length and valid UTF-8 from user, saves us an extra strlen()
						edit_state.CursorAnimReset();
					}
				}
			}

			// Copy back to user buffer
			if (is_editable && strcmp(edit_state.TempTextBuffer.Data, buf) != 0)
			{
				ImStrncpy(buf, edit_state.TempTextBuffer.Data, buf_size);
				value_changed = true;
			}
		}
	}

	// Release active ID at the end of the function (so e.g. pressing Return still does a final application of the value)
	if (clear_active_id && g.ActiveId == id)
		ImGui::ClearActiveID();

	// Render
	// Select which buffer we are going to display. When ImGuiInputTextFlags_NoLiveEdit is set 'buf' might still be the old value. We set buf to NULL to prevent accidental usage from now on.
	const char* buf_display = (g.ActiveId == id && is_editable) ? edit_state.TempTextBuffer.Data : buf;
	buf = NULL;

	if (!is_multiline)
	{
		ImGui::RenderNavHighlight(frame_bb, id);
		ImGui::RenderFrame(frame_bb.Min, frame_bb.Max, Settings::Style::textinput_bg, true, style.FrameRounding);
	}

	const ImVec4 clip_rect(frame_bb.Min.x, frame_bb.Min.y, frame_bb.Min.x + size.x, frame_bb.Min.y + size.y); // Not using frame_bb.Max because we have adjusted size
	ImVec2 render_pos = is_multiline ? draw_window->DC.CursorPos : frame_bb.Min + style.FramePadding;
	ImVec2 text_size(0.f, 0.f);
	const bool is_currently_scrolling = (edit_state.Id == id && is_multiline && g.ActiveId == draw_window->GetIDNoKeepAlive("#SCROLLY"));
	if (g.ActiveId == id || is_currently_scrolling)
	{
		edit_state.CursorAnim += io.DeltaTime;

		// This is going to be messy. We need to:
		// - Display the text (this alone can be more easily clipped)
		// - Handle scrolling, highlight selection, display cursor (those all requires some form of 1d->2d cursor position calculation)
		// - Measure text height (for scrollbar)
		// We are attempting to do most of that in **one main pass** to minimize the computation cost (non-negligible for large amount of text) + 2nd pass for selection rendering (we could merge them by an extra refactoring effort)
		// FIXME: This should occur on buf_display but we'd need to maintain cursor/select_start/select_end for UTF-8.
		const ImWchar* text_begin = edit_state.Text.Data;
		ImVec2 cursor_offset, select_start_offset;

		{
			// Count lines + find lines numbers straddling 'cursor' and 'select_start' position.
			const ImWchar* searches_input_ptr[2];
			searches_input_ptr[0] = text_begin + edit_state.StbState.cursor;
			searches_input_ptr[1] = NULL;
			int searches_remaining = 1;
			int searches_result_line_number[2] = { -1, -999 };
			if (edit_state.StbState.select_start != edit_state.StbState.select_end)
			{
				searches_input_ptr[1] = text_begin + ImMin(edit_state.StbState.select_start, edit_state.StbState.select_end);
				searches_result_line_number[1] = -1;
				searches_remaining++;
			}

			// Iterate all lines to find our line numbers
			// In multi-line mode, we never exit the loop until all lines are counted, so add one extra to the searches_remaining counter.
			searches_remaining += is_multiline ? 1 : 0;
			int line_count = 0;
			for (const ImWchar* s = text_begin; *s != 0; s++)
				if (*s == '\n')
				{
					line_count++;
					if (searches_result_line_number[0] == -1 && s >= searches_input_ptr[0])
					{
						searches_result_line_number[0] = line_count;
						if (--searches_remaining <= 0)
							break;
					}
					if (searches_result_line_number[1] == -1 && s >= searches_input_ptr[1])
					{
						searches_result_line_number[1] = line_count;
						if (--searches_remaining <= 0)
							break;
					}
				}
			line_count++;
			if (searches_result_line_number[0] == -1)
				searches_result_line_number[0] = line_count;
			if (searches_result_line_number[1] == -1)
				searches_result_line_number[1] = line_count;

			// Calculate 2d position by finding the beginning of the line and measuring distance
			cursor_offset.x = InputTextCalcTextSizeW(ImStrbolW(searches_input_ptr[0], text_begin), searches_input_ptr[0]).x;
			cursor_offset.y = searches_result_line_number[0] * g.FontSize;
			if (searches_result_line_number[1] >= 0)
			{
				select_start_offset.x = InputTextCalcTextSizeW(ImStrbolW(searches_input_ptr[1], text_begin), searches_input_ptr[1]).x;
				select_start_offset.y = searches_result_line_number[1] * g.FontSize;
			}

			// Store text height (note that we haven't calculated text width at all, see GitHub issues #383, #1224)
			if (is_multiline)
				text_size = ImVec2(size.x, line_count * g.FontSize);
		}

		// Scroll
		if (edit_state.CursorFollow)
		{
			// Horizontal scroll in chunks of quarter width
			if (!(flags & ImGuiInputTextFlags_NoHorizontalScroll))
			{
				const float scroll_increment_x = size.x * 0.25f;
				if (cursor_offset.x < edit_state.ScrollX)
					edit_state.ScrollX = (float)(int)ImMax(0.0f, cursor_offset.x - scroll_increment_x);
				else if (cursor_offset.x - size.x >= edit_state.ScrollX)
					edit_state.ScrollX = (float)(int)(cursor_offset.x - size.x + scroll_increment_x);
			}
			else
			{
				edit_state.ScrollX = 0.0f;
			}

			// Vertical scroll
			if (is_multiline)
			{
				float scroll_y = draw_window->Scroll.y;
				if (cursor_offset.y - g.FontSize < scroll_y)
					scroll_y = ImMax(0.0f, cursor_offset.y - g.FontSize);
				else if (cursor_offset.y - size.y >= scroll_y)
					scroll_y = cursor_offset.y - size.y;
				draw_window->DC.CursorPos.y += (draw_window->Scroll.y - scroll_y); // To avoid a frame of lag
				draw_window->Scroll.y = scroll_y;
				render_pos.y = draw_window->DC.CursorPos.y;
			}
		}
		edit_state.CursorFollow = false;
		const ImVec2 render_scroll = ImVec2(edit_state.ScrollX, 0.0f);

		// Draw selection
		if (edit_state.StbState.select_start != edit_state.StbState.select_end)
		{
			const ImWchar* text_selected_begin = text_begin + ImMin(edit_state.StbState.select_start, edit_state.StbState.select_end);
			const ImWchar* text_selected_end = text_begin + ImMax(edit_state.StbState.select_start, edit_state.StbState.select_end);

			float bg_offy_up = is_multiline ? 0.0f : -1.0f; // FIXME: those offsets should be part of the style? they don't play so well with multi-line selection.
			float bg_offy_dn = is_multiline ? 0.0f : 2.0f;
			ImU32 bg_color = ImGui::GetColorU32(ImGuiCol_TextSelectedBg);
			ImVec2 rect_pos = render_pos + select_start_offset - render_scroll;
			for (const ImWchar* p = text_selected_begin; p < text_selected_end;)
			{
				if (rect_pos.y > clip_rect.w + g.FontSize)
					break;
				if (rect_pos.y < clip_rect.y)
				{
					while (p < text_selected_end)
						if (*p++ == '\n')
							break;
				}
				else
				{
					ImVec2 rect_size = InputTextCalcTextSizeW(p, text_selected_end, &p, NULL, true);
					if (rect_size.x <= 0.0f)
						rect_size.x = (float)(int)(g.Font->GetCharAdvance((unsigned short)' ') * 0.50f); // So we can see selected empty lines
					ImRect rect(rect_pos + ImVec2(0.0f, bg_offy_up - g.FontSize), rect_pos + ImVec2(rect_size.x, bg_offy_dn));
					rect.ClipWith(clip_rect);
					if (rect.Overlaps(clip_rect))
						draw_window->DrawList->AddRectFilled(rect.Min, rect.Max, bg_color);
				}
				rect_pos.x = render_pos.x - render_scroll.x;
				rect_pos.y += g.FontSize;
			}
		}

		draw_window->DrawList->AddText(g.Font, g.FontSize, render_pos - render_scroll, Settings::Style::textinput_text, buf_display, buf_display + edit_state.CurLenA, 0.0f, is_multiline ? NULL : &clip_rect);

		// Draw blinking cursor
		bool cursor_is_visible = (!g.IO.OptCursorBlink) || (g.InputTextState.CursorAnim <= 0.0f) || ImFmod(g.InputTextState.CursorAnim, 1.20f) <= 0.80f;
		ImVec2 cursor_screen_pos = render_pos + cursor_offset - render_scroll;
		ImRect cursor_screen_rect(cursor_screen_pos.x, cursor_screen_pos.y - g.FontSize + 0.5f, cursor_screen_pos.x + 1.0f, cursor_screen_pos.y - 1.5f);
		if (cursor_is_visible && cursor_screen_rect.Overlaps(clip_rect))
			draw_window->DrawList->AddLine(cursor_screen_rect.Min, cursor_screen_rect.GetBL(), Settings::Style::textinput_text);

		// Notify OS of text input position for advanced IME (-1 x offset so that Windows IME can cover our cursor. Bit of an extra nicety.)
		if (is_editable)
			g.PlatformImePos = ImVec2(cursor_screen_pos.x - 1, cursor_screen_pos.y - g.FontSize);
	}
	else
	{
		// Render text only
		const char* buf_end = NULL;
		if (is_multiline)
			text_size = ImVec2(size.x, InputTextCalcTextLenAndLineCount(buf_display, &buf_end) * g.FontSize); // We don't need width
		draw_window->DrawList->AddText(g.Font, g.FontSize, render_pos, Settings::Style::textinput_text, buf_display, buf_end, 0.0f, is_multiline ? NULL : &clip_rect);
	}

	if (is_multiline)
	{
		ImGui::Dummy(text_size + ImVec2(0.0f, g.FontSize)); // Always add room to scroll an extra line
		ImGui::EndChildFrame();
		ImGui::EndGroup();
	}

	if (is_password)
		ImGui::PopFont();

	// Log as text
	if (g.LogEnabled && !is_password)
		LogRenderedText(&render_pos, buf_display, NULL);

	if (label_size.x > 0)
		ImGui::RenderText(ImVec2(frame_bb.Max.x + style.ItemInnerSpacing.x, frame_bb.Min.y + style.FramePadding.y), label);

	if (value_changed)
		ImGui::MarkItemValueChanged(id);

	if ((flags & ImGuiInputTextFlags_EnterReturnsTrue) != 0)
		return enter_pressed;
	else
		return value_changed;
}
// }}}
