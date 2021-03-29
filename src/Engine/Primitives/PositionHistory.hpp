#ifndef ENGINE_PRIMITIVES_POSITIONHISTORY_HPP
#define ENGINE_PRIMITIVES_POSITIONHISTORY_HPP

#include "Vector.hpp"

struct PositionHistory
{
	// Time stamp for this movement
	f32 animtime;

	Vec3 origin;
	Vec3 angles;
};

#endif // ENGINE_PRIMITIVES_POSITIONHISTORY_HPP
