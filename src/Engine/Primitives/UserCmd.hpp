#ifndef ENGINE_PRIMITIVES_USERCMD_HPP
#define ENGINE_PRIMITIVES_USERCMD_HPP

#include "../../Util/Cenum.hpp"
#include "Vector.hpp"
#include "QAngle.hpp"

struct UserCmd
{
	i16 lerp_msec; // Interpolation time on client
	u8 msec; // Duration in ms of command
	QAngle viewAngles; // Command view angles.

	float forwardMove;    // Forward velocity.
	float sideMove;       // Sideways velocity.
	float upMove;         // Upward velocity.
	u8 lightLevel; // Light level at spot where we are standing.
	u16 buttons; // Attack buttons
	u8 impulse; // Impulse command issued.
	u8 weaponSelect; // Current weapon id

	// Experimental player impact stuff.
	u32 impact_index;
	Vec3 impact_position;
};

MAKE_CENUMV_Q(Buttons, u16,
	IN_ATTACK, 1 << 0,
	IN_JUMP, 1 << 1,
	IN_DUCK, 1 << 2,
	IN_FORWARD, 1 << 3,
	IN_BACK, 1 << 4,
	IN_USE, 1 << 5,
	IN_CANCEL, 1 << 6,
	IN_LEFT, 1 << 7,
	IN_RIGHT, 1 << 8,
	IN_MOVELEFT, 1 << 9,
	IN_MOVERIGHT, 1 << 10,
	IN_ATTACK2, 1 << 11,
	IN_RUN, 1 << 12,
	IN_RELOAD, 1 << 13,
	IN_ALT1, 1 << 14,
	IN_SCORE, 1 << 15,
);

#endif // ENGINE_PRIMITIVES_USERCMD_HPP
