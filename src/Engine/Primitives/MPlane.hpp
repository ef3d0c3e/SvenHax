#ifndef ENGINE_PRIMITIVES_MPLANE_HPP
#define ENGINE_PRIMITIVES_MPLANE_HPP

#include "Vector.hpp"

struct MPlane
{
	Vec3 normal; // surface normal
	f32 dist; // closest appoach to origin
	u8 type; // for texture axis selection and fast side tests
	u8 signbits; // signx + signy<<1 + signz<<1
	u8 pad[2];
};

#endif // ENGINE_PRIMITIVES_MPLANE_HPP
