#ifndef ERNGINE_TEMPENT_HPP
#define ERNGINE_TEMPENT_HPP

#include "Entity.hpp"
#include "Primitives/PMTrace.hpp"

struct TempEnt
{
	i32 flags;
	f32 die;
	f32 frameMax;
	Vec3 pos;
	f32 fadeSpeed;
	f32 bounceFactor;
	i32 hitSound;
	void (*hitCallbackFn)(TempEnt* ent, PMTrace *ptr);
	void (*callbackFn)(TempEnt* ent, f32 frametime, f32 currenttime);
	TempEnt* next;
	i32 priority;
	i16 clientIndex;// if attached, this is the index of the client to stick to
	// if COLLIDEALL, this is the index of the client to ignore
	// TENTS with FTENT_PLYRATTACHMENT MUST set the clientindex! 

	Vec3 tentOffset;// if attached, client origin + tentOffset = tent origin.
	Entity entity;

	// baseline.origin- velocity
	// baseline.renderamt- starting fadeout intensity
	// baseline.angles- angle velocity
};

extern std::array<TempEnt, 2048>* gTempEnts;

#endif // ERNGINE_TEMPENT_HPP
