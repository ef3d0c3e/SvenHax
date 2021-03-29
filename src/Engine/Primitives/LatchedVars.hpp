#ifndef ENGINE_PRIMITIVES_LATCHEDVARS_HPP
#define ENGINE_PRIMITIVES_LATCHEDVARS_HPP

struct LatchedVars
{
	f32 prevAnimTime;
	f32 sequenceTime;
	u8 prevSeqBlending[2];
	Vec3 prevOrigin;
	QAngle prevAngles;

	i32 prevSequence;
	f32 prevFrame;

	u8 prevController[4];
	u8 prevBlending[2];
};

#endif // ENGINE_PRIMITIVES_LATCHEDVARS_HPP
