#include "Widgets.hpp"
#define IMGUI_DEFINE_MATH_OPERATORS
#include "../ImGui/imgui_internal.h"
#include "../ImGui/imgui.cpp"

// {{{ Misc
void RenderFrame(ImVec2 p_min, ImVec2 p_max, ImU32 fill_col, float border_size, float rounding, ImU32 border_col, ImDrawCornerFlags corner_flags = ImDrawCornerFlags_All)
{
    ImGuiContext& g = *GImGui;
    ImGuiWindow* window = g.CurrentWindow;
    window->DrawList->AddRectFilled(p_min, p_max, fill_col, rounding);
    if (border_size > 0.0f)
        window->DrawList->AddRect(p_min, p_max, border_col, rounding, corner_flags, border_size);
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
	ImVec2 size = ImGui::CalcItemSize(size_arg, label_size.x + style.FramePadding.x * 2.0f + label_size.y*6, label_size.y + style.FramePadding.y * 2.0f);

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
	const ImU32 col = (held && hovered)
		? Settings::Style::button_bg[2] : hovered ? Settings::Style::button_bg[1] : Settings::Style::button_bg[0];
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
	const ImU32 col = (held && hovered)
		? Settings::Style::selector_bg[3] : hovered ? Settings::Style::selector_bg[2] : selected
		? Settings::Style::selector_bg[1] : Settings::Style::selector_bg[0];

	ImGui::RenderNavHighlight(bb, id);
	if (selected)
	{
		RenderFrame(bb.Min, bb.Max, col, 1.f, 0, Settings::Style::selector_border);
		window->DrawList->AddRectFilled(bb.Min, ImVec2(bb.Min.x + label_size.y*.3, bb.Max.y), Settings::Style::selector_accent, 0);
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

bool UI::TabList(const char* label, u32 tab, u32 cur, u32 max, const ImVec2& size_arg)
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
	const ImU32 col = (held && hovered)
		? Settings::Style::tablist_bg[3] : hovered ? Settings::Style::tablist_bg[2] : cur == tab
		? Settings::Style::tablist_bg[1] : Settings::Style::tablist_bg[0];

	ImGui::RenderNavHighlight(bb, id);
	if (cur == tab)
	{
		RenderFrame(bb.Min, bb.Max, col, 1.f, 0, Settings::Style::tablist_border, ImDrawCornerFlags_Top | ImDrawCornerFlags_Right);
		window->DrawList->AddRectFilled(bb.Min, ImVec2(bb.Min.x + label_size.y*.3, bb.Max.y), Settings::Style::tablist_accent, 0);

		ImGui::PushStyleColor(ImGuiCol_Text, Settings::Style::tablist_accent);
		ImGui::RenderTextClipped(bb.Min + style.FramePadding, bb.Max - style.FramePadding, label, NULL, &label_size, style.ButtonTextAlign, &bb);
		ImGui::PopStyleColor();
	}
	else
	{
		RenderFrame(bb.Min, bb.Max, col, 0.f, 0, Settings::Style::tablist_border, ImDrawCornerFlags_Top | ImDrawCornerFlags_Right);
		ImGui::RenderTextClipped(bb.Min + style.FramePadding, bb.Max - style.FramePadding, label, NULL, &label_size, style.ButtonTextAlign, &bb);
	}

	return pressed;
}
// }}}

// {{{ Sliders
template<typename TYPE, typename SIGNEDTYPE, typename FLOATTYPE>
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
	if (!is_decimal && v_range >= 0)											 // v_range < 0 may happen on integer overflows
		grab_sz = ImMax((float)(slider_sz / (v_range + 1)), style.GrabMinSize);  // For integer sliders: if possible have the grab size represent 1 unit
	grab_sz = ImMin(grab_sz, slider_sz);
	const float slider_usable_sz = slider_sz - grab_sz;
	const float slider_usable_pos_min = (is_horizontal ? bb.Min.x : bb.Min.y) + grab_padding + grab_sz*0.5f;
	const float slider_usable_pos_max = (is_horizontal ? bb.Max.x : bb.Max.y) - grab_padding - grab_sz*0.5f;

	// For power curve sliders that cross over sign boundary we want the curve to be symmetric around 0.0f
	float linear_zero_pos;   // 0.0->1.0f
	if (is_power && v_min * v_max < 0.0f)
	{
		// Different sign
		const FLOATTYPE linear_dist_min_to_0 = ImPow(v_min >= 0 ? (FLOATTYPE)v_min : -(FLOATTYPE)v_min, (FLOATTYPE)1.0f/power);
		const FLOATTYPE linear_dist_max_to_0 = ImPow(v_max >= 0 ? (FLOATTYPE)v_max : -(FLOATTYPE)v_max, (FLOATTYPE)1.0f/power);
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
				clicked_t = SliderBehaviorCalcRatioFromValue<TYPE,FLOATTYPE>(data_type, *v, v_min, v_max, power, linear_zero_pos);
				const int decimal_precision = is_decimal ? ImParseFormatPrecision(format, 3) : 0;
				if ((decimal_precision > 0) || is_power)
				{
					delta /= 100.0f;	// Gamepad/keyboard tweak speeds in % of slider bounds
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
			v_new = RoundScalarWithFormat<TYPE,SIGNEDTYPE>(format, data_type, v_new);

			// Apply result
			if (*v != v_new)
			{
				*v = v_new;
				value_changed = true;
			}
		}
	}

	// Draw
	float grab_t = SliderBehaviorCalcRatioFromValue<TYPE,FLOATTYPE>(data_type, *v, v_min, v_max, power, linear_zero_pos);
	if (!is_horizontal)
		grab_t = 1.0f - grab_t;
	const float grab_pos = ImLerp(slider_usable_pos_min, slider_usable_pos_max, grab_t);
	ImRect grab_bb;
	if (is_horizontal)
		grab_bb = ImRect(grab_pos - grab_sz*0.5f, bb.Min.y + grab_padding, grab_pos + grab_sz*0.5f, bb.Max.y - grab_padding);
	else
		grab_bb = ImRect(bb.Min.x + grab_padding, grab_pos - grab_sz*0.5f, bb.Max.x - grab_padding, grab_pos + grab_sz*0.5f);


	ImRect rect_bb = bb;
    if (Settings::Style::slider_thickness != 1.0f)
    {
        float shrink_amount = (float)(int)((rect_bb.Max.y - rect_bb.Min.y) * 0.5f * (1.0f - Settings::Style::slider_thickness));
        //bb.Min.y += shrink_amount;
        rect_bb.Max.y -= 2*shrink_amount;
        grab_bb.Max.y -= 2*shrink_amount;
    }
	window->DrawList->AddRectFilled(rect_bb.Min, ImVec2(grab_bb.Min.x + (grab_bb.Max.x - grab_bb.Min.x) * 0.65f, rect_bb.Max.y), Settings::Style::slider_filled, style.FrameRounding, ImDrawCornerFlags_Left);
    window->DrawList->AddRectFilled(ImVec2(grab_bb.Max.x - (grab_bb.Max.x - grab_bb.Min.x) * 0.35f, rect_bb.Min.y), rect_bb.Max, Settings::Style::slider_empty, style.FrameRounding, ImDrawCornerFlags_Right);

	window->DrawList->AddCircleFilled((grab_bb.Min + grab_bb.Max)*.5, Settings::Style::grab_radius, Settings::Style::slider_grab);

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
		IM_ASSERT(*(const ImS32*)v_min >= IM_S32_MIN/2 && *(const ImS32*)v_max <= IM_S32_MAX/2);
		return SliderBehaviorT<ImS32, ImS32, float >(frame_bb, id, data_type, (ImS32*)v,  *(const ImS32*)v_min,  *(const ImS32*)v_max,  format, power, flags);
	case ImGuiDataType_U32:
		IM_ASSERT(*(const ImU32*)v_min <= IM_U32_MAX/2);
		return SliderBehaviorT<ImU32, ImS32, float >(frame_bb, id, data_type, (ImU32*)v,  *(const ImU32*)v_min,  *(const ImU32*)v_max,  format, power, flags);
	case ImGuiDataType_S64:
		IM_ASSERT(*(const ImS64*)v_min >= IM_S64_MIN/2 && *(const ImS64*)v_max <= IM_S64_MAX/2);
		return SliderBehaviorT<ImS64, ImS64, double>(frame_bb, id, data_type, (ImS64*)v,  *(const ImS64*)v_min,  *(const ImS64*)v_max,  format, power, flags);
	case ImGuiDataType_U64:
		IM_ASSERT(*(const ImU64*)v_min <= IM_U64_MAX/2);
		return SliderBehaviorT<ImU64, ImS64, double>(frame_bb, id, data_type, (ImU64*)v,  *(const ImU64*)v_min,  *(const ImU64*)v_max,  format, power, flags);
	case ImGuiDataType_Float:
		IM_ASSERT(*(const float*)v_min >= -FLT_MAX/2.0f && *(const float*)v_max <= FLT_MAX/2.0f);
		return SliderBehaviorT<float, float, float >(frame_bb, id, data_type, (float*)v,  *(const float*)v_min,  *(const float*)v_max,  format, power, flags);
	case ImGuiDataType_Double:
		IM_ASSERT(*(const double*)v_min >= -DBL_MAX/2.0f && *(const double*)v_max <= DBL_MAX/2.0f);
		return SliderBehaviorT<double,double,double>(frame_bb, id, data_type, (double*)v, *(const double*)v_min, *(const double*)v_max, format, power, flags);
	case ImGuiDataType_COUNT: break;
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
	const float w = ImGui::CalcItemWidth();

	const ImVec2 label_size = ImGui::CalcTextSize(label, NULL, true);
	const ImRect frame_bb(window->DC.CursorPos, window->DC.CursorPos + ImVec2(w, label_size.y + style.FramePadding.y * 2.0f));
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

	const ImRect check_bb(window->DC.CursorPos + ImVec2(.25*style.FramePadding.x, .25*style.FramePadding.y), window->DC.CursorPos + ImVec2(label_size.y + 1.5*style.FramePadding.x, label_size.y + 1.5*style.FramePadding.y)); // We want a square shape to we use Y twice
	ImGui::ItemSize(check_bb, style.FramePadding.y);

	ImRect total_bb = check_bb;
	if (label_size.x > 0)
		ImGui::SameLine(0, style.ItemInnerSpacing.x);
	const ImRect text_bb(window->DC.CursorPos + ImVec2(style.FramePadding.y,style.FramePadding.y), window->DC.CursorPos + ImVec2(style.FramePadding.y,style.FramePadding.y) + label_size);
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
	RenderFrame(check_bb.Min, check_bb.Max, (held && hovered)
		? Settings::Style::checkbox_bg[3] : hovered ? Settings::Style::checkbox_bg[2] : *v
		? Settings::Style::checkbox_bg[1] : Settings::Style::checkbox_bg[0], 1.f, 3.f, Settings::Style::checkbox_border);
	if (*v)
	{
		const float check_sz = ImMin(check_bb.GetWidth(), check_bb.GetHeight());
		const float pad = ImMax(1.0f, (float)(int)(check_sz / 6.0f));
		ImGui::RenderCheckMark(check_bb.Min + ImVec2(pad, pad), Settings::Style::checkmark, check_bb.GetWidth() - pad*2.0f);
	}

	if (label_size.x > 0.0f)
		ImGui::RenderText(text_bb.Min, label);

	return pressed;
}
// }}}
