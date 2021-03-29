#ifndef ENGINE_PLAYERMOVE_HPP
#define ENGINE_PLAYERMOVE_HPP

#include "PhysEnt.hpp"
#include "Primitives/MoveVars.hpp"

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

	i32 flags; // FL_ONGROUND, FL_DUCKING, etc.
	i32 useHull; // 0 = regular player hull, 1 = ducked player hull, 2 = point hull
	f32 gravity; // Our current gravity and friction.
	f32 friction;
	i32 oldButtons; // Buttons last usercmd
	f32 waterJumpTime; // Amount of time left in jumping out of water cycle.
	QBool dead; // Are we a dead player?
	i32 deadFlag;
	i32 spectator; // Should we use spectator physics model?
	i32 moveType; // Our movement type, NOCLIP, WALK, FLY

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
	Vector<int, 4> iuser;
	Vector<float, 4> fuser;
	Vector<Vec, 4> vuser;

	// world state
	// Number of entities to clip against.
	i32 numPhysent;
	PhysEnt physents[PhysEnt::Max];
	// Number of momvement entities (ladders)
	i32 numMoveent;
	// just a list of ladders
	PhysEnt moveEnts[MAX_MOVEENTS];

	// All things being rendered, for tracing against things you don't actually collide with
	i32 numBisEnt;
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
	void (*PM_Particle)(float* origin, int color, float life, int zpos, int zvel);
	i32 (*PM_TestPlayerPosition)(float* pos, PMTrace* ptrace);
	void (*Con_NPrintf)(int idx, char* fmt, ...);
	void (*Con_DPrintf)(char* fmt, ...);
	void (*Con_Printf)(char* fmt, ...);
	double (*Sys_FloatTime)(void);
	void (*PM_StuckTouch)(int hitent, PMTrace* ptraceresult);
	i32 (*PM_PointContents)(float* p, int* truecontents); //filled in if this is non-null
	i32 (*PM_TruePointContents)(float* p);
	i32 (*PM_HullPointContents)(struct hull_s* hull, int num, float* p);
	PMTrace (*PM_PlayerTrace)(float* start, float* end, int traceFlags, int ignore_pe);
	struct PMTrace* (*PM_TraceLine)(float* start, float* end, int flags, int usehulll, int ignore_pe);
	long (*RandomLong)(long lLow, long lHigh);
	f32 (*RandomFloat)(float flLow, float flHigh);
	i32 (*PM_GetModelType)(struct model_s* mod);
	void (*PM_GetModelBounds)(struct model_s* mod, float* mins, float* maxs);
	void* (*PM_HullForBsp)(PhysEnt* pe, float* offset);
	f32 (*PM_TraceModel)(PhysEnt* pEnt, float* start, float* end, trace_t* trace);
	i32 (*COM_FileSize)(char* filename);
	u8* (*COM_LoadFile)(char* path, int usehunk, int* pLength);
	void (*COM_FreeFile)(void* buffer);
	char* (*memfgets)(u8* pMemFile, int fileSize, int* pFilePos, char* pBuffer, int bufferSize);

	// Functions
	// Run functions for this frame?
	QBool runFuncs;
	void (*PM_PlaySound)(int channel, const char* sample, float volume, float attenuation, int fFlags, int pitch);
	const char* (*PM_TraceTexture)(int ground, float* vstart, float* vend);
	void (*PM_PlaybackEventFull)(int flags, int clientindex, unsigned short eventindex, float delay, float* origin, float* angles, float fparam1, float fparam2, int iparam1, int iparam2, int bparam1, int bparam2);

	PMTrace (*PM_PlayerTraceEx)(float* start, float* end, int traceFlags, int (*pfnIgnore)(PhysEnt* pe));
	i32 (*PM_TestPlayerPositionEx)(float* pos, PMTrace* ptrace, int (*pfnIgnore)(PhysEnt* pe));
	PMTrace* (*PM_TraceLineEx)(float* start, float* end, int flags, int usehulll, int (*pfnIgnore)(PhysEnt* pe));
	*/
}

#endif // ENGINE_PLAYERMOVE_HPP
