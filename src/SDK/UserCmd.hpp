#ifndef SDK_USERCMD_HPP
#define SDK_USERCMD_HPP

#include "Vector.hpp"
#include "QAngle.hpp"

//TODO!
struct UserCmd
{
	short lerp_msec; // Interpolation time on client
	byte msec; // Duration in ms of command
	QAngle viewangles; // Command view angles.

	//float forwardmove;    // Forward velocity.
	//float sidemove;       // Sideways velocity.
	//float upmove;         // Upward velocity.
	Vec vel; // foward, side, upward
	byte lightlevel; // Light level at spot where we are standing.
	unsigned short buttons; // Attack buttons
	byte impulse; // Impulse command issued.
	byte weaponselect; // Current weapon id

	// Experimental player impact stuff.
	int impact_index;
	Vec impact_position;
};

#endif // SDK_USERCMD_HPP
