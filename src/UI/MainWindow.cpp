#include "MainWindow.hpp"
#include "UI.hpp"
#include <array>
#include <functional>

#include "../ImGUI/imgui.h"

void UI::MainWindow()
{
	if (!UI::isVisible)
		return;

	if (ImGui::Begin("SvenHax"))
	{
		static int tab = 0;
		const std::array<const char*, 4> tabs{
			"AIM",
			"VISUALS",
			"MISC",
			"EXPERIMENTAL",
		};

		for (int i = 0; i < tabs.size(); ++i)
		{
			if (ImGui::Button(tabs[i], ImVec2(ImGui::GetWindowSize().x / tabs.size() - 10, 0)))
				tab = i;

			if (i+1 != tabs.size())
				ImGui::SameLine();
		}

		ImGui::End();
	}
}
