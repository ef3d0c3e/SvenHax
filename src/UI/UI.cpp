#include "UI.hpp"
#include "MainWindow.hpp"
#include <functional>

bool UI::isVisible = true;
void UI::SetVisible(bool visible)
{
	UI::isVisible = visible;
}

void UI::UpdateColors()
{
	auto convert = [](ImU32 col)
	{
		return ImVec4(
			(col >> 16 & 0xFF)/255.f,
			(col >> 8 & 0xFF)/255.f,
			(col & 0xFF)/255.f,
			(col >> 24 & 0xFF)/255.f
		);
	};

	ImVec4* colors = ImGui::GetStyle().Colors;
	colors[ImGuiCol_WindowBg] = convert(Settings::Style::window_bg);
	colors[ImGuiCol_ChildBg] = convert(Settings::Style::child_bg);
}

void UI::Draw()
{
	UI::MainWindow();
}
