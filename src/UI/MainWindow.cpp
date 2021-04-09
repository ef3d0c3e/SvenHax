#include "MainWindow.hpp"
#include "UI.hpp"
#include <array>
#include <functional>

#include "../Engine/Primitives/Defs.hpp"
#define IMGUI_DEFINE_MATH_OPERATORS
#include "../ImGui/imgui_internal.h"
#include "../ImGui/imgui.h"

#include "Image.hpp"
#define INCBIN_PREFIX res
#include "../incbin/incbin.h"
INCBIN(Noa, "../src/UI/Res/noa.png");

void UI::MainWindow()
{
	if (!UI::isVisible)
		return;

	if (ImGui::Begin("SvenHax", NULL, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBringToFrontOnFocus))
	{
		ImGui::PushStyleColor(ImGuiCol_ChildBg, 0xFFF00000);
		ImGui::BeginChild("##TABLIST", ImVec2(0, 58), true);
		ImGui::PopStyleColor();
		static u32 tab = 0;
		const std::array<const char*, 5> tabs{
			"AIM",
			"VISUALS",
			"ANTIAIM",
			"MISC",
			"CONFIG",
		};

		ImGui::Dummy(ImVec2(0, 14));
		ImGui::Dummy(ImVec2(64, 0));
		ImGui::SameLine();
		float w = ImGui::GetContentRegionAvailWidth();
		ImRect bb;
		for (u32 i = 0; i < tabs.size(); ++i)
		{
			if (UI::TabList(tabs[i], tab, i, tabs.size(), bb, ImVec2(w*.14, 44)))
				tab = i;

			ImGui::SameLine();
		}

		// Shadow
		{
			if (tab != 0)
			{
				static ImU32 color [4] = {0, Settings::Style::tablist_shadow, Settings::Style::tablist_shadow, 0,};
				UI::Shadow(bb.Min - ImVec2(bb.Max.x*Settings::Style::tablist_shadow_scale, 0), bb.Min + ImVec2(0, bb.Max.y), color);
			}
			if (tab+1 != tabs.size())
			{
				static ImU32 color[4] = {Settings::Style::tablist_shadow, 0, 0, Settings::Style::tablist_shadow};
				UI::Shadow(ImVec2(bb.Max.x, bb.Min.y), ImVec2(bb.Max.x + bb.Max.x*Settings::Style::tablist_shadow_scale, bb.Max.y), color);
			}
		}

		{
			const float w = bb.Max.x-bb.Min.x;
			const float h = bb.Max.y-bb.Min.y;
			const float y = h+14;
			static ImU32 color[6] = {0, 0, Settings::Style::tablist_shadow, Settings::Style::tablist_shadow, 0, 0}; // cycling
			UI::Shadow(ImGui::GetWindowPos() + ImVec2(0, y-10), ImGui::GetWindowPos() + ImVec2(64+(tab)*w, y), color);
			const float x = 64+(w)*(tab+1);
			UI::Shadow(ImGui::GetWindowPos() + ImVec2(x, y-10), ImGui::GetWindowPos() + ImVec2(ImGui::GetWindowSize().x, y), color);
		}


		ImGui::SetCursorPosY(ImGui::GetCursorPosY()-8);

		ImGui::PushFont(title_font);
		ImGui::SameLine(ImGui::GetCursorPosX() + (ImGui::GetContentRegionAvailWidth() - ImGui::CalcTextSize("CUNNYWARE").x)/2);
		ImGui::PushStyleColor(ImGuiCol_Text, ImGui::ColorConvertU32ToFloat4(Settings::Style::title));
		ImGui::Text("CUNNYWARE");
		ImGui::PopStyleColor();
		ImGui::PopFont();

		ImGui::EndChild();

		ImGui::PushStyleColor(ImGuiCol_ChildBg, ImGui::ColorConvertU32ToFloat4(Settings::Style::tablist_line));
		ImGui::BeginChild("##LINE", ImVec2(0, 6), true);
		ImGui::PopStyleColor();
		ImGui::EndChild();

		switch (tab)
		{
			case 0:
				UI::AimTab();
				break;
			case 2:
				UI::AntiAimTab();
				break;
			case 3:
				UI::MiscTab();
				break;
		}

		const ImVec2 windowPos = ImGui::GetWindowPos();
		const ImVec2 windowSize = ImGui::GetWindowSize();

		ImGui::End();

		static Image noa = []
		{
			Image noa(resNoaData, resNoaSize);
			noa.Load();

			return noa;
		}();

		const ImVec2 pos = ImGui::GetCursorPos();
		const float scale = windowSize.x/noa.GetSize()[0]*.23f;
		const ImVec2 noaSize(noa.GetSize()[0]*scale, noa.GetSize()[1]*scale);
		ImGui::SetCursorPos(windowPos-ImVec2(noaSize.x-windowSize.x+title_font->FontSize, noaSize.y-93*scale));
		ImGui::Image(reinterpret_cast<void*>(noa.GetTexture()), noaSize);
		ImGui::SetCursorPos(pos);
	}
}
