#include "MainWindow.hpp"
#include "Tab.hpp"

#include "../Hacks/AntiAim.hpp"

void GeneralLeft()
{
	Child("##GENERALLEFT", 200);

	ImGui::Text("General");
	UI::Checkbox("Enabled", &Settings::AA::enabled);

	EndChild();

	ImGui::Spacing();

}

void GeneralRight()
{
	Child("##GENERALRIGHT", 400);


	EndChild();
}

void UI::AntiAimTab()
{
	MakeTab
	(
		make_array
		(
			"General"s
		),
		make_array
		(
			GeneralLeft
		),
		make_array
		(
			GeneralRight
		)
	);
}
