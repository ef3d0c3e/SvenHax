#include "MainWindow.hpp"
#include "Tab.hpp"

#include "../Hacks/AntiAim.hpp"

void GeneralLeft()
{
	Child("##AAGENERAL", 130);

	ImGui::Text("General");

	UI::Checkbox("Enabled", &Settings::AA::enabled);
	UI::Checkbox("Clamp", &Settings::AA::clamp);
	UI::Desc("Clamps the angles between\n * [-89, +89] for pitch\n * [-180, +180] for yaw\n * 0 for roll");

	EndChild();

	Child("##AAPITCH", 200);

	ImGui::Text("Picth");

	const static auto pitchTypes = make_array(
		"None"s,
		"Static"s,
		"Jitter"s,
		"Fake (Up)"s,
		"Fake (Down)"s,
		"Lisp"s,
		"Angel (Down)"s,
		"Angel (Up)"s
	);

	if (UI::BeginCombo(""s, pitchTypes[Settings::AA::pitch]))
	{
		for (std::size_t i = 0; i < pitchTypes.size(); ++i)
		{
			if (UI::Selectable(pitchTypes[i].c_str(), Settings::AA::pitch == i))
				Settings::AA::pitch = i;
			if (Settings::AA::pitch == i)
				UI::SetItemDefaultFocus();
		}
		UI::EndCombo();
	}

	switch (Settings::AA::pitch)
	{
		case Settings::AA::Pitch::STATIC:
			ImGui::Text("Value");
			UI::SliderFloat("##VALUE", &Settings::AA::pitchValue, 0.f, -89.f, 89.f, "%.f°");
			UI::Desc("Constant pitch value");
			break;
		case Settings::AA::Pitch::JITTER:
			ImGui::Columns(2, "##JITTERVALUE", false);
			ImGui::Text("Min");
			UI::SliderFloat("##MIN", &Settings::AA::pitchJitterValue[0], 0.f, -89.f, +89.f, "%.f°");
			UI::Desc("Jitter minimum");
			ImGui::NextColumn();
			ImGui::Text("Max");
			UI::SliderFloat("##MAX", &Settings::AA::pitchJitterValue[1], 0.f, -89.f, +89.f, "%.f°");
			UI::Desc("Jitter maximum");
			ImGui::EndColumns();
			break;
		case Settings::AA::Pitch::LISP:
			ImGui::Text("Value");
			UI::SliderFloat("##VALUE", &Settings::AA::pitchValue, 0.f, -89.f, 89.f, "%.f°");
			UI::Desc("Base pitch value");
			break;
		default:
			break;
	}

	EndChild();

	Child("##AAROLL", 150);

	ImGui::Text("Roll");

	const static auto rollTypes = make_array(
		"None"s,
		"Static"s
	);

	if (UI::BeginCombo(""s, rollTypes[Settings::AA::roll]))
	{
		for (std::size_t i = 0; i < rollTypes.size(); ++i)
		{
			if (UI::Selectable(rollTypes[i].c_str(), Settings::AA::roll == i))
				Settings::AA::roll = i;
			if (Settings::AA::roll == i)
				UI::SetItemDefaultFocus();
		}
		UI::EndCombo();
	}

	switch (Settings::AA::roll)
	{
		case Settings::AA::Pitch::STATIC:
			ImGui::Text("Value");
			UI::SliderFloat("##VALUE", &Settings::AA::rollValue, 0.f, -180.f, 180.f, "%.f°");
			UI::Desc("Constant roll value");
			break;
		default:
			break;
	}

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
		"Spin"s,
		"Lisp"s,
		"Angel"s,
		"Angel (Spin)"s
	);

	if (UI::BeginCombo(""s, yawTypes[Settings::AA::yaw]))
	{
		for (std::size_t i = 0; i < yawTypes.size(); ++i)
		{
			if (UI::Selectable(yawTypes[i].c_str(), Settings::AA::yaw == i))
				Settings::AA::yaw = i;
			if (Settings::AA::yaw == i)
				UI::SetItemDefaultFocus();
		}
		UI::EndCombo();
	}

	switch (Settings::AA::yaw)
	{
		case Settings::AA::Yaw::BACKWARD:
			ImGui::Text("Offset");
			UI::SliderFloat("##OFFSET", &Settings::AA::yawOffset, 0.f, -180.f, 180.f, "%.f°");
			UI::Desc("Offset from back yaw");
			break;
		case Settings::AA::Yaw::JITTER:
			ImGui::Text("Offset");
			UI::SliderFloat("##OFFSET", &Settings::AA::yawOffset, 0.f, -180.f, 180.f, "%.f°");
			UI::Desc("Offset from back yaw");

			ImGui::Columns(2, "##JITTERRANGE", false);
			ImGui::Text("Min");
			UI::SliderFloat("##MIN", &Settings::AA::yawJitterRange[0], 0.f, -180.f, 180.f, "%.f°");
			UI::Desc("Jitter minimum offset");
			ImGui::NextColumn();
			ImGui::Text("Max");
			UI::SliderFloat("##MAX", &Settings::AA::yawJitterRange[1], 0.f, -180.f, 180.f, "%.f°");
			UI::Desc("Jitter maximum offset");
			ImGui::EndColumns();
			break;
		case Settings::AA::Yaw::SPIN:
			ImGui::Text("Speed");
			UI::SliderFloat("##SPEED", &Settings::AA::yawSpinSpeed, 0.f, 0.f, 50.f, "%.1f");
			UI::Desc("Adds this much to your yaw every tick");
			break;
		case Settings::AA::Yaw::LISP:
			ImGui::Text("Offset");
			UI::SliderFloat("##OFFSET", &Settings::AA::yawOffset, 0.f, -180.f, 180.f, "%.f°");
			UI::Desc("Offset from back yaw");
			break;
		case Settings::AA::Yaw::ANGEL:
			ImGui::Text("Offset");
			UI::SliderFloat("##OFFSET", &Settings::AA::yawOffset, 0.f, -180.f, 180.f, "%.f°");
			UI::Desc("Offset from back yaw");
			break;
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
