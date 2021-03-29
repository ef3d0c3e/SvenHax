#ifndef SDK_CLIENTDATA_HPP
#define SDK_CLIENTDATA_HPP

#include "Vector.hpp"

struct ClientData
{
	// fields that cannot be modified  (ie. have no effect if changed)
	Vec origin;

	// fields that can be changed by the cldll
	Vec viewangles;
	int		iWeaponBits;
//	int		iAccessoryBits;
	float	fov;	// field of view
};

#endif // SDK_CLIENTDATA_HPP
