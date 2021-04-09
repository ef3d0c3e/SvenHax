#include "Tab.hpp"
#include "MainWindow.hpp"

#include "../Hacks/BHop.hpp"
#include "../Hacks/AutoStrafer.hpp"

void MovementsLeft()
{
	Child("##MOVEMENTSLEFT", 130);

	ImGui::Text("Bunny Hop");
	UI::Checkbox("Enabled", &Settings::BHop::enabled);

	
	EndChild();
}

void MovementsRight()
{
	Child("##MOVEMENTSRIGHT", 250);

	ImGui::Text("Auto strafer");
	UI::Checkbox("Enabled", &Settings::AutoStrafer::enabled);

	static std::string format = "";
	UI::CheckboxCombo({
		{"Silent", "Try to hide strafe angle from other players", Settings::AutoStrafer::silent},
		{"Ground Strafe", "Strafe on ground", Settings::AutoStrafer::ground},
		{"Sideways", "Goes into the direction chose\nby pressing WASD", Settings::AutoStrafer::sideways},
	}, format);

	static ImVec4 color;
	UI::ColorEdit4("Indicator", color, false);
	
	EndChild();
}

void OtherLeft()
{
	Child("##OTHERRIGHT", 250);
	EndChild();
}

void OtherRight()
{

}

void UI::MiscTab()
{
	MakeTab
	(
		make_array
		(
			"Movements"s,
			"Other"s
		),
		make_array
		(
			MovementsLeft,
			OtherLeft
		),
		make_array
		(
			MovementsRight,
			OtherRight
		)
	);
}
