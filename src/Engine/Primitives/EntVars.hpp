#ifndef ENGINE_PRIMITIVES_ENTVARS_HPP
#define ENGINE_PRIMITIVES_ENTVARS_HPP

#include "Defs.hpp"
#include "Vector.hpp"
#include "QAngle.hpp"
struct EDict;

struct EntVars
{
	StringT className;
	StringT globalName;

	Vec3 origin;
	Vec3 oldOrigin;
	Vec3 velocity;
	Vec3 baseVelocity;
	Vec3 clBaseVelocity; // Base velocity that was passed in to server physics so
		//  client can predict conveyors correctly.  Server zeroes it, so we need to store here, too.
	Vec3 moveDir;

	QAngle angles; // Model angles
	QAngle angleVelocity; // angle velocity (degrees per second)
	QAngle punchAngle; // auto-decaying view angle adjustment
	QAngle viewAngle; // Viewing angle (player only)

	// For parametric entities
	Vec3 endPos;
	Vec3 startPos;
	f32 impactTime;
	f32 startTime;

	i32 fixAngle; // 0:nothing, 1:force view angles, 2:add avelocity
	f32 idealPitch;
	f32 pitchSpeed;
	f32 idealYaw;
	f32 yawSpeed;

	i32 modelIndex;
	StringT model;

	i32 viewModel; // player's viewmodel
	i32 weaponModel; // what other players see

	Vec3 absMin; // BB min translated to world coord
	Vec3 absMax; // BB max translated to world coord
	Vec3 mins; // local BB min
	Vec3 maxs; // local BB max
	Vec3 size; // maxs - mins

	f32 lTime; // lerp time?
	f32 nextThink;

	i32 moveType;
	i32 solid;

	i32 skin;
	i32 body; // sub-model selection for studiomodels
	i32 effects;

	f32 gravity; // % of "normal" gravity
	f32 friction; // inverse elasticity of MOVETYPE_BOUNCE

	i32 lightLevel;

	i32 sequence; // animation sequence
	i32 gaitSequence; // movement animation sequence for player (0 for none)
	f32 frame; // % playback position in animation sequences (0..255)
	f32 animTime; // world time when frame was set
	f32 framerate; // animation playback rate (-8x to 8x)
	u8 controller[4]; // bone controller setting (0..255)
	u8 blending[2]; // blending amount between sub-sequences (0..255)

	f32 scale; // sprite rendering scale (0..255)

	i32 renderMode;
	f32 renderAmt;
	Vec3 renderColor;
	i32 renderFx;

	f32 health;
	f32 frags;
	i32 weapons; // bit mask for available weapons
	f32 takeDamage;

	i32 deadFlag;
	Vec3 eyePos; // eye position

	i32 button;
	i32 impulse;

	EDict* chain; // Entity pointer when linked into a linked list
	EDict* dmgInflictor;
	EDict* enemy;
	EDict* aiment; // entity pointer when MOVETYPE_FOLLOW
	EDict* owner;
	EDict* groundEntity;

	i32 spawnFlags;
	i32 flags;

	i32 colormap; // lowbyte topcolor, highbyte bottomcolor
	i32 team;

	f32 maxHealth;
	f32 teleportTime;
	f32 armorType;
	f32 armorValue;
	i32 waterLevel;
	i32 waterType;

	StringT target;
	StringT targetName;
	StringT netName;
	StringT message;

	f32 dmgTake;
	f32 dmgSave;
	f32 dmg;
	f32 dmgTime;

	Vector<StringT, 4> noise;

	f32 speed;
	f32 airFinished;
	f32 painFinished;
	f32 radsuitFinished;

	EDict* containingEntity;

	i32 playerClass;
	f32 maxSpeed;

	f32 fov;
	i32 weaponAnim;

	i32 pushMSec;

	i32 inDuck;
	i32 timeStepSound;
	i32 swimTime;
	i32 duckTime;
	i32 stepLeft;
	f32 fallVelocity;

	i32 gameState;

	i32 oldButtons;

	i32 groupInfo;

	// For mods
	Vector<i32, 4> iuser;
	Vector<f32, 4> fuser;
	Vector<Vec3, 4> vuser;
	Vector<EDict*, 4> euser;
};

#endif // ENGINE_PRIMITIVES_ENTVARS_HPP
