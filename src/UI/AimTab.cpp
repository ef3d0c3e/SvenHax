#include "MainWindow.hpp"
#include "Tab.hpp"

static void general_1()
{
	Child("##CONTENT1", 100);

	ImGui::Text("General1");
	static bool v = false;
	UI::Checkbox("aaa", &v);

	EndChild();

	ImGui::Spacing();

	Child("##CONTENT2", 300);

	ImGui::Text("General2");
	static bool d = false;
	UI::Checkbox("check2", &d);
	UI::Checkbox("check3", &d);

	if (UI::Button("Button"))
	{

	}

	EndChild();
}

static void general_2()
{
	Child("##CONTENT12", 300);

	ImGui::Text("General3");
	static float f = 0.5f;
	UI::SliderFloat("", &f, -180, +180, "%1.f°");

	static char buf[64];
	UI::InputText("in", buf, IM_ARRAYSIZE(buf));
	static char *items[] = {"Dirt", "Cobblestone", "Diamond", "Emerals"};
	static int cur = 1;
	//UI::Combo("Items", &cur, items, IM_ARRAYSIZE(items));
	static const char* current_item = NULL;
	if (ImGui::BeginCombo("LBL", current_item))
	{
		for (int n = 0; n < IM_ARRAYSIZE(items); n++)
		{
			bool is_selected = (current_item == items[n]); // You can store your selection however you want, outside or inside your objects
			if (ImGui::Selectable(items[n], is_selected))
				current_item = items[n];
			if (is_selected)
				ImGui::SetItemDefaultFocus();   // You may set the initial focus when opening the combo (scrolling + for keyboard navigation support)
		}
		ImGui::EndCombo();
	}

	EndChild();
}

void UI::AimTab()
{
	MakeTab
	(
		make_array
		(
			"General"s
		),
		make_array
		(
			general_1
		),
		make_array
		(
			general_2
		)
	);
}
