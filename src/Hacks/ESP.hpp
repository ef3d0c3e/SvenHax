#ifndef HACKS_ESP_HPP
#define HACKS_ESP_HPP

#include "../ImGui/imgui.h"
#include "../Interface.hpp"

namespace ESP
{
	bool WorldToScreen(const Vec3& point, ImVec2& screen);
	void Paint();
}

namespace Settings::ESP
{
	extern bool enabled;
	extern i32 maxDormantTime;

	MAKE_CENUMV_Q(BoxType, u8,
		BOX_2D, 0,
		BOX_2D_FLAT, 1,
		BOX_3D, 2);

	struct EnemySettings
	{
		bool enabled = true;

		ImColor boxColor = ImColor(0.5f, 1.f, 0.f, 1.f);
		BoxType type = BOX_3D;

		ImColor skeletonColor = ImColor(1.f, 1.f, 1.f, 1.f);
		bool skeleton = false;

		ImColor barrelColor = ImColor(1.f, 1.f, 1.f, 1.f);
		bool barrel = false;

		bool healthBar = true;
		bool healthBarRelative = false; // health bar color relative to current/max health

		// Text informations
		bool hp = true;
		bool className = true;
		bool customName = true;
		bool weapon = true;
		bool armor = false;

	};
	//extern std::array<EnemySettings, EnemyEntity.size()> monsters;
}

#endif // HACKS_ESP_HPP
