#ifndef ENGINE_PRIMITIVES_ENTITYSTATE_HPP
#define ENGINE_PRIMITIVES_ENTITYSTATE_HPP

#include "Vector.hpp"

MAKE_CENUM_Q(EntityType, i32,
	NORMAL, 1 << 0,
	BEAM, 1 << 1,
);

struct EntityState
{
	// Fields which are filled in by routines outside of delta compression
	EntityType type;
	// Index into cl_entities array for this entity.
	i32 number;
	f32 msgTime;

	// Message number last time the player/entity state was updated.
	i32 messageNum;

	// Fields which can be transitted and reconstructed over the network stream
	Vec3 origin;
	QAngle angles;

	i32 modelIndex;
	i32 sequence;
	f32 frame;
	i32 colorMap;
	i16 skin;
	i16 solid;
	i32 effects;
	f32 scale;

	u8 eflags;

	// Render information
	i32 renderMode;
	i32 renderAmt;
	Color24 renderColor;
	i32 renderFx;

	i32 moveType;
	f32 animTime;
	f32 frameRate;
	i32 body;
	u8 controller[4];
	u8 blending[4];
	Vec3 velocity;

	// Send bbox down to client for use during prediction.
	Vec3 mins;
	Vec3 maxs;

	i32 aiment;
	// If owned by a player, the index of that player ( for projectiles ).
	i32 owner;

	// Friction, for prediction.
	f32 friction;
	// Gravity multiplier
	f32 gravity;

	// PLAYER SPECIFIC
	i32 team;
	i32 playerClass;
	i32 health;
	QBool spectator;
	i32 weaponModel;
	i32 gaitSequence;
	// If standing on conveyor, e.g.
	Vec3 baseVelocity;
	// Use the crouched hull, or the regular player hull.
	i32 useHull;
	// Latched buttons last time state updated.
	i32 oldButtons;
	// -1 = in air, else pmove entity number
	i32 onGround;
	i32 iStepLeft;
	// How fast we are falling
	f32 fallVelocity;

	f32 fov;
	i32 weaponAnim;

	// Parametric movement overrides
	Vec3 startPos;
	Vec3 endPos;
	f32 impactTime;
	f32 startTime;

	// For mods
	Vector<i32, 4> iuser;
	Vector<f32, 4> fuser;
	Vector<Vec3, 4> vuser;
};

#endif // ENGINE_PRIMITIVES_ENTITYSTATE_HPP
