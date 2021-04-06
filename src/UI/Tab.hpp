#ifndef UI_TAB_HPP
#define UI_TAB_HPP

#define IMGUI_DEFINE_MATH_OPERATORS
#include "../ImGui/imgui.h"
#include "../ImGui/imgui_internal.h"
#include "../Util/Util.hpp"
#include "Style.hpp"
#include "Widgets.hpp"
#include <array>
#include <functional>
#include <string>

using namespace std::literals;

void Child(const char* label, float height);
void EndChild();

/*
+--+ +---+ +---+
|c1| |###| |###|
|c2| +---+ |###|
|c3| +---+ |###|
|  | |###| +---+
|  | +---+ +---+
+--+       |###|
each spacing are .01
*/
template <std::size_t NCategory, class Fn>
__attribute__((always_inline))
void MakeTab(const std::array<std::string, NCategory>& categories,
		const std::array<Fn, NCategory>& left,
		const std::array<Fn, NCategory>& right)
{
	const auto&& [w, h] = ImGui::GetContentRegionAvail();

	static u32 category = 0;

	ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(0, 0));
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
	ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 0);
	ImGui::BeginChild("##SELECTOR", ImVec2(w*.20f, h), true);

	for (u32 i = 0; i < categories.size(); ++i)
	{
		if (UI::SelectButton(categories[i].c_str(), i == category, ImVec2(ImGui::GetContentRegionAvailWidth(), 40)))
			category = i;
	}

	{ // Shadow
		const auto pos = ImVec2(ImGui::GetCursorPos().x + ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionWidth(), ImGui::GetWindowPos().y);
		float y = ImGui::GetCursorPosY();
		ImGui::SetCursorPosY(pos.y);
		static ImU32 color[4] = {0, Settings::Style::selector_shadow, Settings::Style::selector_shadow, 0};
		UI::Shadow(pos-ImVec2(ImGui::GetWindowContentRegionWidth()*Settings::Style::selector_shadow_scale, 0), pos+ImVec2(0, ImGui::GetWindowSize().y), color);
		ImGui::SetCursorPosY(y);
	}

	ImGui::EndChild();
	ImGui::PopStyleVar();
	ImGui::PopStyleVar();
	ImGui::PopStyleVar();

	ImGui::SameLine();
	ImGui::Dummy(ImVec2(w*.01f, 0));
	ImGui::SameLine();

	ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0, 0, 0, 0));
	ImGui::BeginChild("##SCROLL", ImVec2(0, 0), true);
	ImGui::PopStyleColor();
	{
		const auto&& [w, h] = ImGui::GetContentRegionAvail();
		ImGui::Dummy(ImVec2(0, w*.02f));

		ImGui::Columns(2, NULL, false);
		ImGui::SetColumnWidth(0, w*.495f);
		ImGui::SetColumnWidth(1, w*.495f);

		left[category]();

		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(Settings::Style::child_padding, 0));
		ImGui::NextColumn();
		ImGui::PopStyleVar();

		right[category]();
	}
	ImGui::EndChild();
};

#endif // UI_TAB_HPP
