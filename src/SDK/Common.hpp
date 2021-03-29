#ifndef SDK_COMMON_HPP
#define SDK_COMMON_HPP

#include "EngineSurface.hpp" // qboolean
#include "QAngle.hpp"
#include "UserCmd.hpp"
#include "VGuiVertex.hpp"
#include "Vector.hpp"

#include "Primitives.hpp"
#include "Entity.hpp"

// {{{ PhysEnt
struct PhysEnt
{
	static constexpr inline std::size_t Max = 600;
	static constexpr inline std::size_t NameLen = 32;
	char name[NameLen]; // Name of model, or "player" or "world".
	int player;
	Vec origin; // Model's origin in world coordinates.
	Model* model; // only for bsp models
	Model* studiomodel; // SOLID_BBOX, but studio clip intersections.
	Vec mins, maxs; // only for non-bsp models
	int info; // For client or server to use to identify (index into edicts or cl_entities)
	Vec angles; // rotated entities need this info for hull testing to work.

	int solid; // Triggers and func_door type WATER brushes are SOLID_NOT
	int skin; // BSP Contents for such things like fun_door water brushes.
	int rendermode; // So we can ignore glass

	// Complex collision detection.
	float frame;
	int sequence;
	u8 controller[4];
	u8 blending[2];

	int movetype;
	int takedamage;
	int blooddecal;
	int team;
	int classnumber;

	// For mods
	//int iuser1;
	//int iuser2;
	//int iuser3;
	//int iuser4;
	//float fuser1;
	//float fuser2;
	//float fuser3;
	//float fuser4;
	//Vec vuser1;
	//Vec vuser2;
	//Vec vuser3;
	//Vec vuser4;
	Vector<int, 4> iuser;
	Vector<float, 4> fuser;
	Vector<Vec, 4> vuser;
}
// }}}

// {{{ PlayerMove
struct PlayerMove
{
	int player_index; // So we don't try to run the PM_CheckStuck nudging too quickly.
	qboolean server; // For debugging, are we running physics code on server side?

	qboolean multiplayer; // 1 == multiplayer server
	float time; // realtime on host, for reckoning duck timing
	float frametime; // Duration of this frame

	Vec forward, right, up; // Vectors for angles
	// player state
	Vec origin; // Movement origin.
	QAngle angles; // Movement view angles.
	QAngle oldangles; // Angles before movement view angles were looked at.
	Vec velocity; // Current movement direction.
	Vec movedir; // For waterjumping, a forced forward velocity so we can fly over lip of ledge.
	Vec basevelocity; // Velocity of the conveyor we are standing, e.g.

	// For ducking/dead
	Vec view_ofs; // Our eye position.
	float flDuckTime; // Time we started duck
	qboolean bInDuck; // In process of ducking or ducked already?

	// For walking/falling
	int flTimeStepSound; // Next time we can play a step sound
	int iStepLeft;

	float flFallVelocity;
	Vec punchangle;

	float flSwimTime;

	float flNextPrimaryAttack;

	int effects; // MUZZLE FLASH, e.g.

	int flags; // FL_ONGROUND, FL_DUCKING, etc.
	int usehull; // 0 = regular player hull, 1 = ducked player hull, 2 = point hull
	float gravity; // Our current gravity and friction.
	float friction;
	int oldbuttons; // Buttons last usercmd
	float waterjumptime; // Amount of time left in jumping out of water cycle.
	qboolean dead; // Are we a dead player?
	int deadflag;
	int spectator; // Should we use spectator physics model?
	int movetype; // Our movement type, NOCLIP, WALK, FLY

	int onground;
	int waterlevel;
	int watertype;
	int oldwaterlevel;

	char sztexturename[256];
	char chtexturetype;

	float maxspeed;
	float clientmaxspeed; // Player specific maxspeed

	// For mods
	int iuser1;
	int iuser2;
	int iuser3;
	int iuser4;
	float fuser1;
	float fuser2;
	float fuser3;
	float fuser4;
	Vec vuser1;
	Vec vuser2;
	Vec vuser3;
	Vec vuser4;
	// world state
	// Number of entities to clip against.
	int numphysent;
	PhysEnt physents[PhysEnt::Max];
	// Number of momvement entities (ladders)
	int nummoveent;
	// just a list of ladders
	PhysEnt moveents[MAX_MOVEENTS];

	// All things being rendered, for tracing against things you don't actually collide with
	int numvisent;
	PhysEnt visents[PhysEnt::Max];

	// input to run through physics.
	UserCmd cmd;

	// Trace results for objects we collided with.
	int numtouch;
	PMTrace touchindex[PhysEnt::Max];

	char physinfo[MAX_PHYSINFO_STRING]; // Physics info string

	struct movevars_s* movevars;
	Vec player_mins[4];
	Vec player_maxs[4];

	// Common functions
	const char* (*PM_Info_ValueForKey)(const char* s, const char* key);
	void (*PM_Particle)(float* origin, int color, float life, int zpos, int zvel);
	int (*PM_TestPlayerPosition)(float* pos, PMTrace* ptrace);
	void (*Con_NPrintf)(int idx, char* fmt, ...);
	void (*Con_DPrintf)(char* fmt, ...);
	void (*Con_Printf)(char* fmt, ...);
	double (*Sys_FloatTime)(void);
	void (*PM_StuckTouch)(int hitent, PMTrace* ptraceresult);
	int (*PM_PointContents)(float* p, int* truecontents /*filled in if this is non-null*/);
	int (*PM_TruePointContents)(float* p);
	int (*PM_HullPointContents)(struct hull_s* hull, int num, float* p);
	PMTrace (*PM_PlayerTrace)(float* start, float* end, int traceFlags, int ignore_pe);
	struct PMTrace* (*PM_TraceLine)(float* start, float* end, int flags, int usehulll, int ignore_pe);
	long (*RandomLong)(long lLow, long lHigh);
	float (*RandomFloat)(float flLow, float flHigh);
	int (*PM_GetModelType)(struct model_s* mod);
	void (*PM_GetModelBounds)(struct model_s* mod, float* mins, float* maxs);
	void* (*PM_HullForBsp)(PhysEnt* pe, float* offset);
	float (*PM_TraceModel)(PhysEnt* pEnt, float* start, float* end, trace_t* trace);
	int (*COM_FileSize)(char* filename);
	u8* (*COM_LoadFile)(char* path, int usehunk, int* pLength);
	void (*COM_FreeFile)(void* buffer);
	char* (*memfgets)(u8* pMemFile, int fileSize, int* pFilePos, char* pBuffer, int bufferSize);

	// Functions
	// Run functions for this frame?
	qboolean runfuncs;
	void (*PM_PlaySound)(int channel, const char* sample, float volume, float attenuation, int fFlags, int pitch);
	const char* (*PM_TraceTexture)(int ground, float* vstart, float* vend);
	void (*PM_PlaybackEventFull)(int flags, int clientindex, unsigned short eventindex, float delay, float* origin, float* angles, float fparam1, float fparam2, int iparam1, int iparam2, int bparam1, int bparam2);

	PMTrace (*PM_PlayerTraceEx)(float* start, float* end, int traceFlags, int (*pfnIgnore)(PhysEnt* pe));
	int (*PM_TestPlayerPositionEx)(float* pos, PMTrace* ptrace, int (*pfnIgnore)(PhysEnt* pe));
	struct PMTrace* (*PM_TraceLineEx)(float* start, float* end, int flags, int usehulll, int (*pfnIgnore)(PhysEnt* pe));
};
// }}}

// {{{ Model
MAKE_CENUM_Q(ModelType int,
	BRUSH, 0,
	SPRITE, 1,
	ALIAS, 2,
	STUDIO 3,
);

MAKE_CENUM_Q(SyncType int,
	SYNC, 0,
	RAND, 1,
);
struct Model
{
	static constexpr inline std::size_t MaxNameLen = 64;
	char name[MaxNameLen];
	qboolean needload; // bmodels and sprites don't cache normally

	ModelType type;
	int numframes;
	SyncType synctype;

	int flags;

	//
	// volume occupied by the model
	//
	Vec mins, maxs;
	float radius;

	//
	// brush model
	//
	int firstmodelsurface, nummodelsurfaces;

	int numsubmodels;
	DModel* submodels;

	int numplanes;
	MPlane* planes;

	int numleafs; // number of visible leafs, not counting 0
	MLeaf* leafs;

	int numvertexes;
	MVertex* vertexes;

	int numedges;
	MEdge* edges;

	int numnodes;
	MNode* nodes;

	int numtexinfo;
	MTexInfo* texinfo;

	int numsurfaces;
	MSurface* surfaces;

	int numsurfedges;
	int* surfedges;

	int numclipnodes;
	DClipNode* clipnodes;

	int nummarksurfaces;
	MSurface** marksurfaces;

	Hull hulls[Hull::MaxMapHulls];

	int numtextures;
	Texture** textures;

	u8* visdata;

	Color24* lightdata;

	char* entities;

	//
	// additional model data
	//
	CacheUser cache; // only access through Mod_Extradata
}
// }}}

// {{{ UserCmd
struct UserCmd
{
	short lerp_msec; // Interpolation time on client
	u8 msec; // Duration in ms of command
	QAngle viewangles; // Command view angles.

	//float forwardmove;    // Forward velocity.
	//float sidemove;       // Sideways velocity.
	//float upmove;         // Upward velocity.
	Vec vel; // foward, side, upward
	u8 lightlevel; // Light level at spot where we are standing.
	unsigned short buttons; // Attack buttons
	u8 impulse; // Impulse command issued.
	u8 weaponselect; // Current weapon id

	// Experimental player impact stuff.
	int impact_index;
	Vec impact_position;
};
// }}}

#endif // SDK_COMMON_HPP
