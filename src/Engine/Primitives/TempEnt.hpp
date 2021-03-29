#ifndef ENGINE_PRIMITIVES_TEMPENT_HPP
#define ENGINE_PRIMITIVES_TEMPENT_HPP

#include "Vector.hpp"
#include "PMTrace.hpp"
struct Entity;

struct TempEnt
{
	i32 flags;
	f32 die;
	f32 frameMax;
	Vec3 pos;
	f32 fadeSpeed;
	f32 bounceFactor;
	i32 hitSound;
	void (*hitCallback)(TempEnt* ent, PMTrace* ptr);
	void (*callback)(TempEnt* ent, f32 frameTime, f32 currentTime);
	TempEnt* next;
	i32 priority;
	i16 clientIndex; // if attached, this is the index of the client to stick to
	// if COLLIDEALL, this is the index of the client to ignore
	// TENTS with FTENT_PLYRATTACHMENT MUST set the clientindex!

	Vec3 tentOffset; // if attached, client origin + tentOffset = tent origin.
	Entity entity;

	// baseline.origin		- velocity
	// baseline.renderamt	- starting fadeout intensity
	// baseline.angles		- angle velocity
};

#endif // ENGINE_PRIMITIVES_TEMPENT_HPP
