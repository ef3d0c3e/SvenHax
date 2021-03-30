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
	void* (*Mem_Calloc)(i32 number, size_t size);
	// Check to see if pointer is in the cache
	void* (*Cache_Check)(CacheUser* c);
	// Load file into cache ( can be swapped out on demand )
	void (*LoadCacheFile)(char* path, CacheUser* cu);
	// Retrieve model pointer for the named model
	Model* (*Mod_ForName)(const char* name, i32 crash_if_missing);
	// Retrieve pointer to studio model data block from a model
	void* (*Mod_Extradata)(Model* mod);
	// Retrieve indexed model from client side model precache list
	Model* (*GetModelByIndex)(i32 index);
	// Get entity that is set for rendering
	Entity* (*GetCurrentEntity)(void);
	// Get referenced player_info_t
	PlayerInfo* (*PlayerInfo)(i32 index);
	// Get most recently received player state data from network system
	EntityState* (*GetPlayerState)(i32 index);
	// Get viewentity
	Entity* (*GetViewEntity)(void);
	// Get current frame count, and last two timestampes on client
	void (*GetTimes)(int* framecount, double* current, double* old);
	// Get a pointer to a cvar by name
	CVar* (*GetCvar)(const char* name);
	// Get current render origin and view vectors ( up, right and vpn )
	void (*GetViewInfo)(f32* origin, f32* upv, f32* rightv, f32* vpnv);
	// Get sprite model used for applying chrome effect
	Model* (*GetChromeSprite)(void);
	// Get model counters so we can incement instrumentation
	void (*GetModelCounters)(int** s, int** a);
	// Get software scaling coefficients
	void (*GetAliasScale)(f32* x, f32* y);

	// Get bone, light, alias, and rotation matrices
	f32**** (*StudioGetBoneTransform)(void);
	f32**** (*StudioGetLightTransform)(void);
	f32*** (*StudioGetAliasTransform)(void);
	f32*** (*StudioGetRotationMatrix)(void);

	// Set up body part, and get submodel pointers
	void (*StudioSetupModel)(i32 bodypart, void** ppbodypart, void** ppsubmodel);
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
	void (*StudioSetupSkin)(void* ptexturehdr, i32 index);
	// Sets up for remapped colors
	void (*StudioSetRemapColors)(i32 top, i32 bottom);
	// Set's player model and returns model pointer
	Model* (*SetupPlayerModel)(i32 index);
	// Fires any events embedded in animation
	void (*StudioClientEvents)(void);
	// Retrieve/set forced render effects flags
	i32 (*GetForceFaceFlags)(void);
	void (*SetForceFaceFlags)(i32 flags);
	// Tell engine the value of the studio model header
	void (*StudioSetHeader)(void* header);
	// Tell engine which model_t * is being renderered
	void (*SetRenderModel)(Model* model);

	// Final state setup and restore for rendering
	void (*SetupRenderer)(i32 rendermode);
	void (*RestoreRenderer)(void);

	// Set render origin for applying chrome effect
	void (*SetChromeOrigin)(void);

	// True if using D3D/OpenGL
	i32 (*IsHardware)(void);

	// Only called by hardware interface
	void (*GL_StudioDrawShadow)(void);
	void (*GL_SetRenderMode)(i32 mode);

	void (*StudioSetRenderamt)(i32 iRenderamt); //!!!CZERO added for rendering glass on viewmodels
	void (*StudioSetCullState)(i32 iCull);
	void (*StudioRenderShadow)(i32 iSprite, f32* p1, f32* p2, f32* p3, f32* p4);
};

#endif // ENGINE_ENGINESTUDIOAPI_HPP
