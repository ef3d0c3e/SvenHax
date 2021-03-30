#ifndef ENGINE_PHYSENT_HPP
#define ENGINE_PHYSENT_HPP

#include "Model.hpp"

struct PhysEnt
{
	static constexpr inline std::size_t Max = 600;
	static constexpr inline std::size_t NameLen = 32;
	char name[NameLen]; // Name of model, or "player" or "world".
	i32 player;
	Vec3 origin; // Model's origin in world coordinates.
	Model* model; // only for bsp models
	Model* studiomodel; // SOLID_BBOX, but studio clip intersections.
	Vec3 mins, maxs; // only for non-bsp models
	i32 info; // For client or server to use to identify (index into edicts or cl_entities)
	QAngle angles; // rotated entities need this info for hull testing to work.

	i32 solid; // Triggers and func_door type WATER brushes are SOLID_NOT
	i32 skin; // BSP Contents for such things like fun_door water brushes.
	i32 rendermode; // So we can ignore glass

	// Complex collision detection.
	f32 frame;
	i32 sequence;
	u8 controller[4];
	u8 blending[2];

	i32 movetype;
	i32 takedamage;
	i32 blooddecal;
	i32 team;
	i32 classnumber;

	// For mods
	Vector<i32, 4> iuser;
	Vector<f32, 4> fuser;
	Vector<Vec3, 4> vuser;
}

#endif // ENGINE_PHYSENT_HPP
