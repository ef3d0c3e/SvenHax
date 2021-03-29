#ifndef ENGINE_PRIMITIVES_USERCMD_HPP
#define ENGINE_PRIMITIVES_USERCMD_HPP

#include "../../Util/Cenum.hpp"
#include "Vector.hpp"
#include "QAngle.hpp"

struct UserCmd
{
	i16 lerp_msec; // Interpolation time on client
	u8 msec; // Duration in ms of command
	QAngle viewangles; // Command view angles.

	//float forwardmove;    // Forward velocity.
	//float sidemove;       // Sideways velocity.
	//float upmove;         // Upward velocity.
	MAKE_CENUMV_Q(MoveDirection, u8,
		FORWARD, 0,
		SIDE, 1,
		UP, 2,
	);
	Vec3 vel; // foward, side, upward
	u8 lightlevel; // Light level at spot where we are standing.
	u16 buttons; // Attack buttons
	u8 impulse; // Impulse command issued.
	u8 weaponselect; // Current weapon id

	// Experimental player impact stuff.
	u32 impact_index;
	Vec3 impact_position;
};

#endif // ENGINE_PRIMITIVES_USERCMD_HPP
