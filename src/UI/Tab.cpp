#include "Tab.hpp"

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
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 8));
}

void EndChild()
{
	ImGui::PopStyleVar();
	ImGui::EndChild();
	ImGui::PopStyleColor();
	ImGui::EndChild();
	ImGui::Dummy(ImVec2(0, Settings::Style::child_padding));
}
