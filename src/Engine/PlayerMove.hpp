#ifndef ENGINE_PLAYERMOVE_HPP
#define ENGINE_PLAYERMOVE_HPP

#include "PhysEnt.hpp"
#include "Primitives/MoveVars.hpp"
#include "Primitives/UserCmd.hpp"
#include "Primitives/PMTrace.hpp"

MAKE_CENUM_Q(MoveFlags, i32,
	FLY, 1<<0,           // Changes the SV_Movestep() behavior to not need to be on ground
	SWIM, 1<<1,          // Changes the SV_Movestep() behavior to not need to be on ground (but stay in water)
	CONVEYOR, 1<<2,
	CLIENT, 1<<3,
	INWATER, 1<<4,
	MONSTER, 1<<5,
	GODMODE, 1<<6,
	NOTARGET, 1<<7,
	SKIPLOCALHOST, 1<<8, // Don't send entity to local host, it's predicting this entity itself
	ONGROUND, 1<<9,      // At rest / on the ground
	PARTIALGROUND, 1<<10,// not all corners are valid
	WATERJUMP, 1<<11,    // player jumping out of water
	FROZEN, 1<<12,       // Player is frozen for 3rd person camera
	FAKECLIENT, 1<<13,   // JAC: fake client, simulated server side; don't send network messages to them
	DUCKING, 1<<14,      // Player flag -- Player is fully crouched
	FLOAT, 1<<15,        // Apply floating force to this entity when in water
	GRAPHED, 1<<16,      // worldgraph has this ent listed as something that blocks a connection
	IMMUNE_WATER, 1<<17,
	IMMUNE_SLIME, 1<<18,
	IMMUNE_LAVA, 1<<19,
	PROXY, 1<<20,        // This is a spectator proxy
	ALWAYSTHINK, 1<<21,  // Brush model flag -- call think every frame regardless of nextthink - ltime (for constantly changing velocity/path)
	BASEVELOCITY, 1<<22, // Base velocity has been applied this frame (used to convert base velocity into momentum)
	MONSTERCLIP, 1<<23,  // Only collide in with monsters who have FL_MONSTERCLIP set
	ONTRAIN, 1<<24,      // Player is _controlling_ a train, so movement commands should be ignored on client during prediction.
	WORLDBRUSH, 1<<25,   // Not moveable/removeable brush entity (really part of the world, but represented as an entity for transparency or something)
	SPECTATOR, 1<<26,    // This client is a spectator, don't run touch functions, etc.
	CUSTOMENTITY, 1<<29, // This is a custom entity
	KILLME, 1<<30,       // This entity is marked for death -- This allows the engine to kill ents at the appropriate time
	DORMANT, 1<<31,      // Entity is dormant, no updates to client
);

MAKE_CENUM_Q(MoveType, i32,
	NONE, 0,           // never moves
	ANGLENOCLIP, 1,
	ANGLECLIP, 2,
	WALK, 3,           // Player only - moving on the ground
	STEP, 4,           // gravity, special edge handling -- monsters use this
	FLY, 5,            // No gravity, but still collides with stuff (also ladder)
	TOSS, 6,           // gravity/collisions
	PUSH, 7,           // no clip to world, push and crush
	NOCLIP, 8,         // No gravity, no collisions, still do velocity/avelocity
	FLYMISSILE, 9,     // extra size to monsters
	BOUNCE, 10,        // Just like Toss, but reflect velocity when contacting surfaces
	BOUNCEMISSILE, 11, // bounce w/o gravity
	FOLLOW, 12,        // track movement of aiment
	PUSHSTEP, 13,      // BSP model that needs physics/world collisions (uses nearest hull for world collision)
);

struct PlayerMove
{
	i32 player_index; // So we don't try to run the PM_CheckStuck nudging too quickly.
	QBool server; // For debugging, are we running physics code on server side?

	QBool multiplayer; // 1 == multiplayer server
	f32 time; // realtime on host, for reckoning duck timing
	f32 frametime; // Duration of this frame

	Vec3 forward, right, up; // Vectors for angles
	// player state
	Vec3 origin; // Movement origin.
	QAngle angles; // Movement view angles.
	QAngle oldAngles; // Angles before movement view angles were looked at.
	Vec3 velocity; // Current movement direction.
	Vec3 moveDir; // For waterjumping, a forced forward velocity so we can fly over lip of ledge.
	Vec3 baseBelocity; // Velocity of the conveyor we are standing, e.g.

	// For ducking/dead
	Vec3 viewOfs; // Our eye position.
	f32 duckTime; // Time we started duck
	QBool inDuck; // In process of ducking or ducked already?

	// For walking/falling
	i32 timeStepSound; // Next time we can play a step sound
	i32 stepLeft;

	f32 fallVelocity;
	QAngle punchAngle;

	f32 swimTime;

	f32 nextPrimaryAttack;

	i32 effects; // MUZZLE FLASH, e.g.

	MoveFlags flags; // FL_ONGROUND, FL_DUCKING, etc.
	i32 useHull; // 0 = regular player hull, 1 = ducked player hull, 2 = point hull
	f32 gravity; // Our current gravity and friction.
	f32 friction;
	i32 oldButtons; // Buttons last usercmd
	f32 waterJumpTime; // Amount of time left in jumping out of water cycle.
	QBool dead; // Are we a dead player?
	i32 deadFlag;
	i32 spectator; // Should we use spectator physics model?
	MoveType moveType; // Our movement type, NOCLIP, WALK, FLY

	i32 onGround;
	i32 waterLevel;
	i32 waterType;
	i32 oldWaterLevel;

	constexpr static inline std::size_t TextureNameLen = 256;
	char textureName[TextureNameLen];
	char chTextureType;

	f32 maxSpeed;
	f32 clientMaxSpeed; // Player specific maxspeed

	// For mods
	Vector<i32, 4> iuser;
	Vector<f32, 4> fuser;
	Vector<Vec3, 4> vuser;

	// world state
	// Number of entities to clip against.
	i32 numPhysent;
	PhysEnt physents[PhysEnt::Max];
	// Number of momvement entities (ladders)
	i32 numMoveent;
	// just a list of ladders
	static constexpr inline std::size_t MaxMoveEnts = 64;
	PhysEnt moveEnts[MaxMoveEnts];

	// All things being rendered, for tracing against things you don't actually collide with
	i32 numVisEnt;
	PhysEnt visEnts[PhysEnt::Max];

	// input to run through physics.
	UserCmd cmd;

	// Trace results for objects we collided with.
	i32 numTouch;
	PMTrace touchIndex[PhysEnt::Max];

	static constexpr inline std::size_t MaxPhysinfoLen = 256;
	char physInfo[MaxPhysinfoLen]; // Physics info string

	MoveVars* moveVars;
	Vec3 playerMins[4];
	Vec3 playerMaxs[4];

	/* TODO: Do something about these
	// Common functions
	const char* (*PM_Info_ValueForKey)(const char* s, const char* key);
	void (*PM_Particle)(f32* origin, i32 color, f32 life, i32 zpos, i32 zvel);
	i32 (*PM_TestPlayerPosition)(f32* pos, PMTrace* ptrace);
	void (*Con_NPrintf)(int idx, char* fmt, ...);
	void (*Con_DPrintf)(char* fmt, ...);
	void (*Con_Printf)(char* fmt, ...);
	double (*Sys_FloatTime)(void);
	void (*PM_StuckTouch)(int hitent, PMTrace* ptraceresult);
	i32 (*PM_PointContents)(f32* p, int* truecontents); //filled in if this is non-null
	i32 (*PM_TruePointContents)(f32* p);
	i32 (*PM_HullPointContents)(struct hull_s* hull, i32 num, f32* p);
	PMTrace (*PM_PlayerTrace)(f32* start, f32* end, i32 traceFlags, i32 ignore_pe);
	struct PMTrace* (*PM_TraceLine)(f32* start, f32* end, i32 flags, i32 usehulll, i32 ignore_pe);
	long (*RandomLong)(long lLow, long lHigh);
	f32 (*RandomFloat)(f32 flLow, f32 flHigh);
	i32 (*PM_GetModelType)(struct model_s* mod);
	void (*PM_GetModelBounds)(struct model_s* mod, f32* mins, f32* maxs);
	void* (*PM_HullForBsp)(PhysEnt* pe, f32* offset);
	f32 (*PM_TraceModel)(PhysEnt* pEnt, f32* start, f32* end, trace_t* trace);
	i32 (*COM_FileSize)(char* filename);
	u8* (*COM_LoadFile)(char* path, i32 usehunk, int* pLength);
	void (*COM_FreeFile)(void* buffer);
	char* (*memfgets)(u8* pMemFile, i32 fileSize, int* pFilePos, char* pBuffer, i32 bufferSize);

	// Functions
	// Run functions for this frame?
	QBool runFuncs;
	void (*PM_PlaySound)(int channel, const char* sample, f32 volume, f32 attenuation, i32 fFlags, i32 pitch);
	const char* (*PM_TraceTexture)(int ground, f32* vstart, f32* vend);
	void (*PM_PlaybackEventFull)(int flags, i32 clientindex, unsigned short eventindex, f32 delay, f32* origin, f32* angles, f32 fparam1, f32 fparam2, i32 iparam1, i32 iparam2, i32 bparam1, i32 bparam2);

	PMTrace (*PM_PlayerTraceEx)(f32* start, f32* end, i32 traceFlags, i32 (*pfnIgnore)(PhysEnt* pe));
	i32 (*PM_TestPlayerPositionEx)(f32* pos, PMTrace* ptrace, i32 (*pfnIgnore)(PhysEnt* pe));
	PMTrace* (*PM_TraceLineEx)(f32* start, f32* end, i32 flags, i32 usehulll, i32 (*pfnIgnore)(PhysEnt* pe));
	*/
};

extern PlayerMove* gPMove;

#endif // ENGINE_PLAYERMOVE_HPP
