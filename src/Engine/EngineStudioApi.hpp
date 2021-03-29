#ifndef ENGINE_ENGINESTUDIOAPI_HPP
#define ENGINE_ENGINESTUDIOAPI_HPP

#include "Entity.hpp"
#include "Models.hpp"
#include "Primitives/CVar.hpp"
#include "Primitives/CacheUser.hpp"
#include "Primitives/EntityState.hpp"
#include "Primitives/PlayerInfo.hpp"

struct EngineStudioApi
{
	// Allocate number*size bytes and zero it
	void* (*Mem_Calloc)(int number, size_t size);
	// Check to see if pointer is in the cache
	void* (*Cache_Check)(CacheUser* c);
	// Load file into cache ( can be swapped out on demand )
	void (*LoadCacheFile)(char* path, CacheUser* cu);
	// Retrieve model pointer for the named model
	Model* (*Mod_ForName)(const char* name, int crash_if_missing);
	// Retrieve pointer to studio model data block from a model
	void* (*Mod_Extradata)(Model* mod);
	// Retrieve indexed model from client side model precache list
	Model* (*GetModelByIndex)(int index);
	// Get entity that is set for rendering
	Entity* (*GetCurrentEntity)(void);
	// Get referenced player_info_t
	PlayerInfo* (*PlayerInfo)(int index);
	// Get most recently received player state data from network system
	EntityState* (*GetPlayerState)(int index);
	// Get viewentity
	Entity* (*GetViewEntity)(void);
	// Get current frame count, and last two timestampes on client
	void (*GetTimes)(int* framecount, double* current, double* old);
	// Get a pointer to a cvar by name
	CVar* (*GetCvar)(const char* name);
	// Get current render origin and view vectors ( up, right and vpn )
	void (*GetViewInfo)(float* origin, float* upv, float* rightv, float* vpnv);
	// Get sprite model used for applying chrome effect
	Model* (*GetChromeSprite)(void);
	// Get model counters so we can incement instrumentation
	void (*GetModelCounters)(int** s, int** a);
	// Get software scaling coefficients
	void (*GetAliasScale)(float* x, float* y);

	// Get bone, light, alias, and rotation matrices
	float**** (*StudioGetBoneTransform)(void);
	float**** (*StudioGetLightTransform)(void);
	float*** (*StudioGetAliasTransform)(void);
	float*** (*StudioGetRotationMatrix)(void);

	// Set up body part, and get submodel pointers
	void (*StudioSetupModel)(int bodypart, void** ppbodypart, void** ppsubmodel);
	// Check if entity's bbox is in the view frustum
	int (*StudioCheckBBox)(void);
	// Apply lighting effects to model
	void (*StudioDynamicLight)(Entity* ent, ALight* plight);
	void (*StudioEntityLight)(ALight* plight);
	void (*StudioSetupLighting)(ALight* plighting);

	// Draw mesh vertices
	void (*StudioDrawPoints)(void);

	// Draw hulls around bones
	void (*StudioDrawHulls)(void);
	// Draw bbox around studio models
	void (*StudioDrawAbsBBox)(void);
	// Draws bones
	void (*StudioDrawBones)(void);
	// Loads in appropriate texture for model
	void (*StudioSetupSkin)(void* ptexturehdr, int index);
	// Sets up for remapped colors
	void (*StudioSetRemapColors)(int top, int bottom);
	// Set's player model and returns model pointer
	Model* (*SetupPlayerModel)(int index);
	// Fires any events embedded in animation
	void (*StudioClientEvents)(void);
	// Retrieve/set forced render effects flags
	int (*GetForceFaceFlags)(void);
	void (*SetForceFaceFlags)(int flags);
	// Tell engine the value of the studio model header
	void (*StudioSetHeader)(void* header);
	// Tell engine which model_t * is being renderered
	void (*SetRenderModel)(Model* model);

	// Final state setup and restore for rendering
	void (*SetupRenderer)(int rendermode);
	void (*RestoreRenderer)(void);

	// Set render origin for applying chrome effect
	void (*SetChromeOrigin)(void);

	// True if using D3D/OpenGL
	int (*IsHardware)(void);

	// Only called by hardware interface
	void (*GL_StudioDrawShadow)(void);
	void (*GL_SetRenderMode)(int mode);

	void (*StudioSetRenderamt)(int iRenderamt); //!!!CZERO added for rendering glass on viewmodels
	void (*StudioSetCullState)(int iCull);
	void (*StudioRenderShadow)(int iSprite, float* p1, float* p2, float* p3, float* p4);
};

#endif // ENGINE_ENGINESTUDIOAPI_HPP
