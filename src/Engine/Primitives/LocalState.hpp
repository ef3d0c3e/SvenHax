#ifndef ENGINE_PRIMITIVES_LOCALSTATE_HPP
#define ENGINE_PRIMITIVES_LOCALSTATE_HPP

#include "ClientData.hpp"
#include "EntityState.hpp"
#include "WeaponData.hpp"

struct LocalState
{
	EntityState playerState;
	ClientData client;
	static constexpr inline std::size_t WeaponDataLen = 64;
	WeaponData weaponData[WeaponDataLen];
};

#endif // ENGINE_PRIMITIVES_LOCALSTATE_HPP
