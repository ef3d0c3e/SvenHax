#ifndef ENGINE_PRIMITIVES_STUDIOINTERFACE_HPP
#define ENGINE_PRIMITIVES_STUDIOINTERFACE_HPP

#include "Defs.hpp"
#include "EntityState.hpp"


struct StudioInterface
{
	i32 version;
	i32 (*StudioDrawModel)(i32 flags);
	i32 (*StudioDrawPlayer)(i32 flags, EntityState* player);
};

#endif // ENGINE_PRIMITIVES_STUDIOINTERFACE_HPP
