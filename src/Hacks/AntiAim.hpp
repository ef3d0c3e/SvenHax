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
	SPIN, 3,
	LISP, 4,
	ANGEL, 5,
	ANGEL_INVERSE, 6,
	ANGEL_SPIN, 7);
extern Yaw yaw;
extern f32 yawOffset;
extern f32 yawJitterRange[2];
extern f32 yawSpinSpeed;

MAKE_CENUM_Q(Pitch, u8,
	NONE, 0,
	STATIC, 1,
	JITTER, 2,
	FAKE_UP, 3,
	FAKE_DOWN, 4,
	LISP, 5,
	ANGEL_DOWN, 6,
	ANGEL_UP, 7);
extern Pitch pitch;
extern f32 pitchValue;
extern f32 pitchJitterValue[2];

MAKE_CENUM_Q(Roll, u8,
	NONE, 0,
	STATIC, 1);
extern Roll roll;
extern f32 rollValue;
}

#endif // HACKS_ANTIAIM_HPP
