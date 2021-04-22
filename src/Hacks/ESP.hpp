#ifndef HACKS_ESP_HPP
#define HACKS_ESP_HPP

#include "../ImGui/imgui.h"
#include "../Interface.hpp"

namespace ESP
{
	bool WorldToScreen(Vec3& point, ImVec2& screen);
	void Paint();
}

namespace Settings::ESP
{
	extern bool enabled;
}

#endif // HACKS_ESP_HPP
