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

	MAKE_CENUMV_Q(BoxType, u8,
		BOX_2D, 0,
		BOX_2D_FLAT, 1,
		BOX_3D, 2);

	MAKE_CENUMV_Q(EnemyEntity, u8,
		AH64, 1,
		ALIEN_CONTROLLER, 2,
		ALIEN_GRUNT, 3,
		ALIEN_SLAVE, 4,
		BABY_GARGANTUA, 5,
		BABY_HEADCRAB, 6,
		BABY_VOLTIGORE, 7,
		BARNACLE, 8,
		BODYGUARD, 9,
		BULLSQUID, 10,
		CHUMTOAD, 11,
		FEMALE_ASSASSIN, 12,
		GARGANTUA, 13,
		GENE_WORM, 14,
		GONARCH, 15,
		GONOME, 16,
		GRUNT, 17,
		HEADCRAB, 18,
		HOUNDEYE, 19,
		ITCHYOSAUR, 20,
		KINGPIN, 21,
		LARGE_TURRET, 22,
		LEECH, 23,
		MALE_ASSASSIN, 24,
		NIHILANTH, 25,
		PIT_DRONE, 26,
		PIT_WORM, 27,
		ROBO_GRUNT, 28,
		SENTRY, 29,
		SERGEANT, 30,
		SHOCK_ROACH, 31,
		SHOCK_TROOPER, 32,
		SMALL_TURRET, 33,
		SNARK, 34,
		STUKABAT, 35,
		TENTACLE, 36,
		TRIPMINE, 37,
		VOLTIGURE, 38,
		XEN_COMMANDER, 39,
		ZOMBIE, 40,
	);

	constexpr static std::array<std::string_view, EnemyEntity::size> EnemyEntityClass
	{
		// Their name always start with 'monster_', so we omit it
		"*TODO* AH64",
		"alien_controller",
		"alien_grunt",
		"alien_slave",
		"babygarg",
		"baby_hearcrab", //?
		"baby_voltigore",
		"barnacle",
		"bodyguard",
		"bullchicken",
		"chumtoad",
		"female_assassin", //?
		"gargantua",
		"geneworm",
		"gonarch",
		"gonome",
		"human_grunt",
		"headcrab",
		"houndeye",
		"itchyosaur", // ?
		"kingpin",
		"turret",
		"leech", //?
		"male_assassin",
		"nihilanth", //?
		"pitdrone",
		"pitworm", //?
		"robogrunt",
		"sentry",
		"hwgrunt",
		"shockroach",
		"shocktrooper",
		"*todo* small turret",
		"snark",
		"stukabat",
		"tentacle", //?
		"tripmine",
		"alien_voltigore",
		"alien_tor",
		"zombie", // ?
	};

	constexpr static std::array<std::string_view, EnemyEntity::size> EnemiesEntitiesDisplayName
	{
		"Apache Helicopter",
		"Alien Controller",
		"Alien Grunt",
		"Alien Slave",
		"Baby Gargantua",
		"Baby Headcrab", //?
		"Baby Voltigore",
		"Barnacle",
		"Bodyguard",
		"Bullsquid",
		"Chumtoad",
		"Female Assassin", //?
		"Gargantua",
		"Gene Worm",
		"Gonarch",
		"Gonome",
		"Human Grunt",
		"Headcrab",
		"Houndeye",
		"Itchyosaur", // ?
		"Kingpin",
		"Large Turret",
		"Leech", //?
		"Male Assassin",
		"Nihilanth", //?
		"Pitdrone",
		"Pitworm", //?
		"Robo Grunt",
		"Sentry",
		"Sergeant",
		"Shock Roach",
		"Shock Trooper",
		"Small Turret",
		"Snark",
		"Stukabat",
		"Tentacle", //?
		"Tripmine",
		"Alien Voltigore",
		"Xen Commander",
		"Zombie", // ?
	};


	//TODO:  monster_human_torch_ally, monster_human_ally, monster_human_medic_ally

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
