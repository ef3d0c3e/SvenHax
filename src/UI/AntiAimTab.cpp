#include "MainWindow.hpp"
#include "Tab.hpp"

#include "../Hacks/AntiAim.hpp"

void GeneralLeft()
{
	Child("##AAGENERAL", 130);

	ImGui::Text("General");

	UI::Checkbox("Enabled", &Settings::AA::enabled);
	UI::Checkbox("Clamp", &Settings::AA::clamp);
	UI::Desc("Clamps the angles between\n * [-89, +89] for pitch\n * [-180, +180] for yaw");

	EndChild();

	Child("##AAPITCH", 200);

	ImGui::Text("Picth");

	EndChild();
}

void GeneralRight()
{
	Child("##AAYAW", 400);

	ImGui::Text("Yaw");


	const static auto yawTypes = make_array(
		"None"s,
		"Backward"s,
		"Jitter"s,
		"Spin (Slow)"s,
		"Spin (Fast)"s,
		"Lisp"s,
		"Lisp (Side)"s,
		"Lisp (Jitter)"s,
		"Angel (Backward)"s,
		"Angel (Inverse)"s,
		"Angel (Spin)"s
	);

	if (UI::BeginCombo(""s, yawTypes[Settings::AA::yaw]))
	{
		for (std::size_t i = 0; i < yawTypes.size(); ++i)
		{
			if (UI::Selectable( std::string(yawTypes[i]).c_str(), Settings::AA::yaw == i))
				Settings::AA::yaw = i;
			if (Settings::AA::yaw == i)
				UI::SetItemDefaultFocus();
		}
		UI::EndCombo();
	}

	switch (Settings::AA::yaw)
	{
		case Settings::AA::Yaw::JITTER:
			ImGui::Text("Offset");
			UI::SliderFloat("##OFFSET", &Settings::AA::yawOffset, 0.f, -180.f, 180.f, "%.f°");
			UI::Desc("Offset from current yaw");

			ImGui::Columns(2, "##JITTERRANGE", false);
			ImGui::Text("Min");
			UI::SliderFloat("##MIN", &Settings::AA::yawJitterRange[0], 0.f, -180.f, 0.f, "%.f°");
			UI::Desc("Jitter absolute minimum");
			ImGui::NextColumn();
			ImGui::Text("Max");
			UI::SliderFloat("##MAX", &Settings::AA::yawJitterRange[1], 0.f, 0.f, +180.f, "%.f°");
			UI::Desc("Jitter absolute maximum");
			ImGui::EndColumns();

		default:
			break;
	}

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
