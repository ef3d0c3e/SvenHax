#include "MainWindow.hpp"
#include "UI.hpp"
#include <array>
#include <functional>

#include "../Engine/Primitives/Defs.hpp"
#include "../ImGui/imgui.h"

void UI::MainWindow()
{
	if (!UI::isVisible)
		return;

	if (ImGui::Begin("SvenHax", NULL, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar))
	{
		ImGui::BeginChild("##TABLIST", ImVec2(0, 58), true);
		static u32 tab = 0;
		const std::array<const char*, 4> tabs{
			"AIM",
			"VISUALS",
			"MISC",
			"EXPERIMENTAL",
		};

		ImGui::Dummy(ImVec2(0, 14));
		ImGui::Dummy(ImVec2(64, 0));
		ImGui::SameLine();
		float w = ImGui::GetContentRegionAvailWidth();
		for (u32 i = 0; i < tabs.size(); ++i)
		{
			if (UI::TabList(tabs[i], tab, i, tabs.size(), ImVec2(w*.17, 44)))
				tab = i;

			ImGui::SameLine();
		}

		ImGui::SameLine();
		ImGui::Dummy(ImVec2(64, 0));
		ImGui::SameLine();
		ImGui::SetCursorPosY(ImGui::GetCursorPosY()-8);

		ImGui::PushFont(title_font);
		ImGui::PushStyleColor(ImGuiCol_Text, ImGui::ColorConvertU32ToFloat4(Settings::Style::title));
		ImGui::Text("SvenHax");
		ImGui::PopStyleColor();
		ImGui::PopFont();

		ImGui::EndChild();

		ImGui::PushStyleColor(ImGuiCol_ChildBg, ImGui::ColorConvertU32ToFloat4(Settings::Style::tablist_line));
		ImGui::BeginChild("##LINE", ImVec2(0, 4), true);
		ImGui::PopStyleColor();
		ImGui::EndChild();

		switch (tab)
		{
			case 0:
				UI::AimTab();
				break;
		}

		ImGui::End();
	}
}
