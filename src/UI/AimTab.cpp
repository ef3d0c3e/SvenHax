#include "MainWindow.hpp"
#include <array>

void Child(const char* label, float height)
{
	ImGui::BeginChild(label, ImVec2(0, height), true, ImGuiWindowFlags_NoScrollbar);
	ImGui::Dummy(ImVec2(0, Settings::Style::child_padding));
	ImGui::Dummy(ImVec2(Settings::Style::child_padding, 0));
	ImGui::SameLine();
	const auto&& [w, h] = ImGui::GetContentRegionAvail();
	ImGui::BeginChild(label, ImVec2(w-Settings::Style::child_padding, h-Settings::Style::child_padding), true, ImGuiWindowFlags_NoScrollbar);
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 16));
}

void EndChild()
{
	ImGui::PopStyleVar();
	ImGui::EndChild();
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
	UI::SliderFloat("Yaw", &f, -180, +180, "%1.f°");

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
	{
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
