#ifndef ENGINE_PRIMITIVES_CLIENTDATA_HPP
#define ENGINE_PRIMITIVES_CLIENTDATA_HPP

#include "Vector.hpp"
#include "QAngle.hpp"

struct ClientData
{
	// fields that cannot be modified  (ie. have no effect if changed)
	Vec3 origin;

	// fields that can be changed by the cldll
	QAngle viewAngles;
	i32 iWeaponBits;
	//	i32		iAccessoryBits;
	f32 fov; // field of view
};

#endif // ENGINE_PRIMITIVES_CLIENTDATA_HPP
