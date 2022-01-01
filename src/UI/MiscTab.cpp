#include "Tab.hpp"
#include "MainWindow.hpp"

#include "../Hacks/BHop.hpp"
#include "../Hacks/AutoStrafer.hpp"
#include "../Hacks/RainbowModel.hpp"

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
	Export<"autostrafer.enabled">(Settings::AutoStrafer::enabled);

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
	ImGui::Text("Rainbow Model");

	UI::Checkbox("Top", &Settings::RainbowModel::top);
	Export<"rainbowmodel.top">(Settings::RainbowModel::top);
	ImGui::Text("Top Speed");
	UI::SliderFloat("##TOP_SPEED", &Settings::RainbowModel::topSpeed, 0.f, 0.f, 8.f, "%.2f");
	Export<"rainbowmodel.topSpeed">(Settings::RainbowModel::topSpeed);
	UI::Desc("Color rotation speed");

	UI::Checkbox("Bottom", &Settings::RainbowModel::bottom);
	Export<"rainbowmodel.bottom">(Settings::RainbowModel::bottom);
	ImGui::Text("Bottom Speed");
	UI::SliderFloat("##BOTTOM_SPEED", &Settings::RainbowModel::bottomSpeed, 0.f, 0.f, 8.f, "%.2f");
	Export<"rainbowmodel.bottomSpeed">(Settings::RainbowModel::bottomSpeed);
	UI::Desc("Color rotation speed");

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
