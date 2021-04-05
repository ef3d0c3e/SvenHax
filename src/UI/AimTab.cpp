#define IMGUI_DEFINE_MATH_OPERATORS
#include "../ImGui/imgui.h"
#include "../ImGui/imgui_internal.h"
#include "MainWindow.hpp"
#include <array>

void Child(const char* label, float height)
{
	ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 4);
	ImGui::BeginChild(label, ImVec2(0, height), true, ImGuiWindowFlags_NoScrollbar);
	ImGui::Dummy(ImVec2(0, Settings::Style::child_padding));
	ImGui::Dummy(ImVec2(Settings::Style::child_padding, 0));
	ImGui::SameLine();
	const auto&& [w, h] = ImGui::GetContentRegionAvail();
	ImGui::PopStyleVar();
	ImGui::PushStyleColor(ImGuiCol_ChildBg, 0x00000000);
	ImGui::BeginChild(label, ImVec2(w-Settings::Style::child_padding, h-Settings::Style::child_padding), true, ImGuiWindowFlags_NoScrollbar);
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 16));
}

void EndChild()
{
	ImGui::PopStyleVar();
	ImGui::EndChild();
	ImGui::PopStyleColor();
	ImGui::EndChild();
	ImGui::Dummy(ImVec2(0, Settings::Style::child_padding));
}

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

/*
+--+ +---+ +---+
|c1| |###| |###|
|c2| +---+ |###|
|c3| +---+ |###|
|  | |###| +---+
|  | +---+ +---+
+--+       |###|
each spacing are .01
*/
void UI::AimTab()
{
	const auto&& [w, h] = ImGui::GetContentRegionAvail();

	static u32 category = 0;

	ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(0, 0));
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
	ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 0);
	ImGui::BeginChild("##SELECTOR", ImVec2(w*.20f, h), true);

	const static std::array<const char*, 3> categories{
		"General",
		"Target",
		"Accuracy",
	};

	for (u32 i = 0; i < categories.size(); ++i)
	{
		if (UI::SelectButton(categories[i], i == category, ImVec2(ImGui::GetContentRegionAvailWidth(), 40)))
			category = i;
	}

	{ // Shadow
		const auto pos = ImVec2(ImGui::GetCursorPos().x + ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionWidth(), ImGui::GetWindowPos().y);
		float y = ImGui::GetCursorPosY();
		ImGui::SetCursorPosY(pos.y);
		static ImU32 color[4] = {0, Settings::Style::selector_shadow, Settings::Style::selector_shadow, 0};
		UI::Shadow(pos-ImVec2(ImGui::GetWindowContentRegionWidth()*Settings::Style::selector_shadow_scale, 0), pos+ImVec2(0, ImGui::GetWindowSize().y), color);
		ImGui::SetCursorPosY(y);
	}

	ImGui::EndChild();
	ImGui::PopStyleVar();
	ImGui::PopStyleVar();
	ImGui::PopStyleVar();

	ImGui::SameLine();
	ImGui::Dummy(ImVec2(w*.01f, 0));
	ImGui::SameLine();

	ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0, 0, 0, 0));
	ImGui::BeginChild("##SCROLL", ImVec2(0, 0), true);
	ImGui::PopStyleColor();
	{
		const auto&& [w, h] = ImGui::GetContentRegionAvail();
		ImGui::Dummy(ImVec2(0, w*.02f));

		ImGui::Columns(2, NULL, false);
		ImGui::SetColumnWidth(0, w*.495f);
		ImGui::SetColumnWidth(1, w*.495f);


		switch (category)
		{
			case 0:
				general_1();
				break;
		}

		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(Settings::Style::child_padding, 0));
		ImGui::NextColumn();
		ImGui::PopStyleVar();

		switch (category)
		{
			case 0:
				general_2();
				break;
		}
	}
	ImGui::EndChild();
}
