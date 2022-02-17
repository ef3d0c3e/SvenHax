#ifndef HACKS_INFO_HPP
#define HACKS_INFO_HPP

#include "../Interface.hpp"
#include <chrono>

namespace Info
{
	MAKE_CENUM_Q(EntityClass, u16,
		NONE, 0,
		PLAYER, 1,
		SCIENTIST, 2,
		BARNEY, 3,
		OTIS, 4,
		HEADCRAB, 5,
		ZOMBIE, 6,
		BULLSQUID, 7,
		HOUNDEYE, 8,
		BARNACLE, 9,
		VORTIGAUNT, 10,
		HUMAN_GRUNT, 11,
		ALIENT_GRUNT, 12,
		TENTACLE, 13,
		SENTRY, 14,
		TURRET, 15,
		LEECH, 16,
		GMAN, 17,
		FEMALE_ASSASSIN, 18,
		MALE_ASSASSIN, 19,
		SNARK, 20,
		CHUMTOAD, 21,
		ALIEN_CONTROLLER, 22,
		ICHTYOSAUR, 23,
		GARGANTUA, 24,
		BABY_GARGANTUA, 25,
		BIG_MOMMA, 26,
		OSPREY, 27,
		APACHE, 28,
		NIHILANTH, 29,
		HEV, 30,
		XEN_TREE, 31,
		XEN_FUNGUS, 32,
		HUMAN_GRUNT_OPFOR, 33,
		GONOME, 34,
		PIT_DRONE, 35,
		SHOCK_TROOPER, 36,
		VOLTIGORE, 37,
		BABY_VOLTIGORE, 38,
		PIT_WORM, 39,
		SHOCK_RIFLE, 40,
		SPORE_AMMO, 41,
		HEAVY_GRUNT, 42,
		ROBOT_GRUNT, 43,
	);

	MAKE_CENUM_Q(ClassFlags, u16,
		FRIEND, (1 << 0),
		ENEMY, (1 << 1),
		NEUTRAL, (1 << 2),
		WORLD_ENTITY, (1 << 3),
		DEAD_BODY, (1 << 4),
		UNDEFINED_6, (1 << 5),
		UNDEFINED_7, (1 << 6),
		UNDEFINED_8, (1 << 7),
		UNDEFINED_9, (1 << 8),
		UNDEFINED_10, (1 << 9),
		UNDEFINED_11, (1 << 10),
		UNDEFINED_12, (1 << 11),
		UNDEFINED_13, (1 << 12),
		UNDEFINED_14, (1 << 13),
		UNDEFINED_15, (1 << 14),
		UNDEFINED_16, (1 << 15),
	);

	struct EntInfo
	{
		Entity& ent;

		EntityClass classId;
		ClassFlags classFlags;

		EntInfo(Entity* ent);
		~EntInfo() {}

		std::chrono::time_point<std::chrono::high_resolution_clock> lastUpdated;
		bool dormant;

		ClassFlags GetClassFlags() const noexcept;
		EntityClass GetClass() const noexcept;
		std::string GetClassName() const noexcept;
		bool IsFriend() const noexcept;
		bool IsEnemy() const noexcept;
		bool IsNeutral() const noexcept;
		bool IsWorldEntity() const noexcept;
		bool IsDeadbody() const noexcept;
	};

	extern std::map<i32, EntInfo> entityInfo;
	extern std::map<std::string_view, std::pair<EntityClass, ClassFlags>> modelMap;

	/**
	 * Populates the model map
	 *
	 * TODO: Call this after map/level change & parse models from bsp
	 * NOTE: Currently called from main function
	 */
	void PopulateModelMap();

	void Paint();
}

#endif // HACKS_INFO_HPP
