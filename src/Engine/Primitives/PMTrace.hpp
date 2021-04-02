#ifndef ENGINE_PRIMITIVES_PMTRACE_HPP
#define ENGINE_PRIMITIVES_PMTRACE_HPP

#include "Vector.hpp"
#include "Defs.hpp"
#include "PMPlane.hpp"

struct PMTrace
{
	QBool allsolid; // if true, plane is not valid
	QBool startsolid; // if true, the initial point was in a solid area
	QBool inopen, inwater; // End point is in empty space or in water
	f32 fraction; // time completed, 1.0 = didn't hit anything
	Vec3 endpos; // final position
	PMPlane plane; // surface normal at impact
	i32 ent; // entity at impact
	Vec3 deltavelocity; // Change in player's velocity caused by impact.
	// Only run on server.
	i32 hitgroup;
};

#endif // ENGINE_PRIMITIVES_PMTRACE_HPP
