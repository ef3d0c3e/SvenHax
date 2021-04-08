#ifndef HACKS_ANTIAIM_HPP
#define HACKS_ANTIAIM_HPP

#include "../Engine/Primitives/QAngle.hpp"
#include "../Engine/Primitives/UserCmd.hpp"

namespace AntiAim
{
	extern QAngle previousAngle;

	void CreateMove(UserCmd* cmd);
}

namespace Settings::AA
{
extern bool enabled;
extern bool clamp;
MAKE_CENUM_Q(Yaw, u8,
	NONE, 0,
	BACKWARD, 1,
	JITTER, 2,
	SPIN_SLOW, 3,
	SPIN_FAST, 4,
	LISP, 5,
	LISP_SIDE, 6,
	LISP_JITTER, 7,
	ANGEL_BACKWARD, 8,
	ANGEL_INVERSE, 9,
	ANGEL_SPIN, 10);
extern Yaw yaw;
extern float yawOffset;
extern float yawJitterRange[2];

MAKE_CENUM_Q(Pitch, u8,
	NONE, 0,
	STATIC, 1,
	JITTER, 2,
	FAKE_UP, 3,
	FAKE_DOWN, 4,
	LISP_DOWN, 5,
	ANGEL_DOWN, 6,
	ANGEL_UP, 7);
extern Pitch pitch;
}

#endif // HACKS_ANTIAIM_HPP
