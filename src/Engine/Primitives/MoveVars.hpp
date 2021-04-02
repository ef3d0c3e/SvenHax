#ifndef ENGINE_PRIMITIVES_MOVEVARS_HPP
#define ENGINE_PRIMITIVES_MOVEVARS_HPP

#include "Color.hpp"
#include "Vector.hpp"

struct MoveVars
{
	f32 gravity; // Gravity for map
	f32 stopSpeed; // Deceleration when not moving
	f32 maxSpeed; // Max allowed speed
	f32 spectatorMaxSpeed;
	f32 accelerate; // Acceleration factor
	f32 airAccelerate; // Same for when in open air
	f32 waterAccelerate; // Same for when in water
	f32 friction;
	f32 edgeFriction; // Extra friction near dropofs
	f32 waterFriction; // Less in water
	f32 entGravity; // 1.0
	f32 bounce; // Wall bounce value. 1.0
	f32 stepSize; // sv_stepsize;
	f32 maxVelocity; // maximum server velocity.
	f32 zMax; // Max z-buffer range (for GL)
	f32 waveHeight; // Water wave height (for GL)
	QBool footsteps; // Play footstep sounds
	static constexpr inline std::size_t skyNameLen = 32;
	char skyName[skyNameLen]; // Name of the sky map
	f32 rollAngle;
	f32 rollSpeed;
	Color3f skyColor; // rgb
	Vec3 skyVec;
};

#endif // ENGINE_PRIMITIVES_MOVEVARS_HPP
