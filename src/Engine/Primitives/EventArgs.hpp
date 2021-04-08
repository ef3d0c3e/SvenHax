#ifndef ENGINE_PRIMITIVES_EVENTARGS_HPP
#define ENGINE_PRIMITIVES_EVENTARGS_HPP

#include "Defs.hpp"
#include "Vector.hpp"
#include "QAngle.hpp"

struct EventArgs
{
	i32 flags;

	// Transmitted
	i32 entIndex;

	Vec3 origin;
	QAngle anlges;
	Vec3 velocity;

	i32 ducking;

	Vector<f32, 2> fparam;
	Vector<i32, 2> iparam;
	Vector<QBool, 2> bparam;
};

#endif // ENGINE_PRIMITIVES_EVENTARGS_HPP
