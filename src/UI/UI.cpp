#include "UI.hpp"
#include "MainWindow.hpp"

bool UI::isVisible = true;
void UI::SetVisible(bool visible)
{
	UI::isVisible = visible;
}

void UI::Draw()
{
	UI::MainWindow();
}
