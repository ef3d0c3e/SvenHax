#ifndef ENGINE_PRIMITIVES_RESOURCE_HPP
#define ENGINE_PRIMITIVES_RESOURCE_HPP

#include "Defs.hpp"
#include <array>

MAKE_CENUM_Q(ResourceType, i32,
	SOUND, 0,
	SKIN, 1,
	MODEL, 2,
	DECAL, 3,
	GENERIC, 4,
	EVENTSCRIPT, 5,
	WORLD, 6, // Fake type for world, is really t_model
);

struct Resource
{
	static constexpr inline std::size_t QPathLen = 64;
	std::array<char, QPathLen> fileName;
	ResourceType type; // t_sound, t_skin, t_model, t_decal.
	i32 index; // For t_decals
	i32 downloadSize; // Size in Bytes if this must be downloaded.
	u8 flags;

	// For handling client to client resource propagation
	std::array<u8, 16> rgucMD5Hash; // To determine if we already have it.
	u8 playernum; // Which player index this resource is associated with, if it's a custom resource.

	std::array<u8, 32> rgucReserved; // For future expansion
	Resource *next; // Next in chain.
	Resource *nprev;
};

#endif // ENGINE_PRIMITIVES_RESOURCE_HPP
