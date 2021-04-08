#ifndef ENGINE_ENGINE_HPP
#define ENGINE_ENGINE_HPP

#include "Primitives/Defs.hpp"
#include "Primitives/ClientSprite.hpp"
#include "Primitives/WRect.hpp"
#include "Primitives/ScreenInfo.hpp"
#include "Primitives/CVar.hpp"
#include "Primitives/HUDPlayerInfo.hpp"
#include "Primitives/ClientTextMessage.hpp"
#include "Primitives/ConNPrint.hpp"
#include "Primitives/Vector.hpp"
#include "Primitives/QAngle.hpp"
#include "Primitives/PMTrace.hpp"
#include "Primitives/EDict.hpp"
#include "Primitives/EventArgs.hpp"
#include "Primitives/ScreenFade.hpp"
#include "Primitives/TagPoint.hpp"
#include "../Util/CStructs.hpp"

#include "Entity.hpp"

struct EngineFuncs
{
	HSprite (*pfnSPR_Load)(const char* szPicName);
	i32 (*pfnSPR_Frames)(HSprite hPic);
	i32 (*pfnSPR_Height)(HSprite hPic, i32 frame);
	i32 (*pfnSPR_Width)(HSprite hPic, i32 frame);
	void (*pfnSPR_Set)(HSprite hPic, i32 r, i32 g, i32 b);
	void (*pfnSPR_Draw)(i32 frame, i32 x, i32 y, const WRect* prc);
	void (*pfnSPR_DrawHoles)(i32 frame, i32 x, i32 y, const WRect* prc);
	void (*pfnSPR_DrawAdditive)(i32 frame, i32 x, i32 y, const WRect* prc);
	void (*pfnSPR_EnableScissor)(i32 x, i32 y, i32 width, i32 height);
	void (*pfnSPR_DisableScissor)(void);
	ClientSprite* (*pfnSPR_GetList)(char* psz, i32* piCount);

	// screen handlers
	void (*pfnFillRGBA)(i32 x, i32 y, i32 width, i32 height, i32 r, i32 g, i32 b, i32 a);
	i32 (*pfnGetScreenInfo)(ScreenInfo* pscrinfo);
	void (*pfnSetCrosshair)(HSprite hspr, WRect rc, i32 r, i32 g, i32 b);

	// cvar handlers
	CVar* (*pfnRegisterVariable)(char* name, char* value, int flags);
	f32 (*pfnGetCvarFloat)(char* name);
	char* (*pfnGetCvarString)(char* name);

	// command handlers
	i32 (*pfnAddCommand)(char* cmd_name, void (*function)(void));
	i32 (*pfnHookUserMsg)(char* szMsgName, i32 (*userMsgHook)(const char* name, i32 size, void* buf));
	i32 (*pfnServerCmd)(char* cmdString);
	i32 (*pfnClientCmd)(char* cmdString);

	void (*pfnGetPlayerInfo)(i32 ent_num, HUDPlayerInfo* pinfo);

	// sound handlers
	void (*pfnPlaySoundByName)(char* sound, f32 volume);
	void (*pfnPlaySoundByIndex)(i32 sound, f32 volume);

	// vector helpers
	void (*pfnAngleVectors)(const f32* vecAngles, f32* forward, f32* right, f32* up);

	// text message system
	ClientTextMessage* (*pfnTextMessageGet)(const char* name);
	i32 (*pfnDrawCharacter)(i32 x, i32 y, i32 number, i32 r, i32 g, i32 b);
	i32 (*pfnDrawConsoleString)(i32 x, i32 y, char* string);
	void (*pfnDrawSetTextColor)(f32 r, f32 g, f32 b);
	void (*pfnDrawConsoleStringLen)(const char* string, i32* length, i32* height);

	void (*pfnConsolePrint)(const char* string);
	void (*pfnCenterPrint)(const char* string);


	// Added for user input processing
	i32 (*GetWindowCenterX)(void);
	i32 (*GetWindowCenterY)(void);
	void (*GetViewAngles)(f32*);
	void (*SetViewAngles)(f32*);
	i32 (*GetMaxClients)(void);
	void (*Cvar_SetValue)(char* cvar, f32 value);

	i32 (*Cmd_Argc)(void);
	char* (*Cmd_Argv)(i32 arg);
	void (*Con_Printf)(char* fmt, ...);
	void (*Con_DPrintf)(char* fmt, ...);
	void (*Con_NPrintf)(i32 pos, char* fmt, ...);
	void (*Con_NXPrintf)(struct ConNPrint* info, char* fmt, ...);

	const char* (*PhysInfo_ValueForKey)(const char* key);
	const char* (*ServerInfo_ValueForKey)(const char* key);
	f32 (*GetClientMaxspeed)(void);
	i32 (*CheckParm)(char* parm, char** ppnext);
	void (*Key_Event)(i32 key, i32 down);
	void (*GetMousePosition)(i32* mx, i32* my);
	i32 (*IsNoClipping)(void);

	Entity* (*GetLocalPlayer)(void);
	Entity* (*GetViewModel)(void);
	Entity* (*GetEntityByIndex)(i32 idx);

	f32 (*GetClientTime)(void);
	void (*V_CalcShake)(void);
	void (*V_ApplyShake)(Vec3& origin, QAngle& angles, f32 factor);

	i32 (*PM_PointContents)(f32* point, i32* truecontents);
	i32 (*PM_WaterEntity)(f32* p);
	PMTrace* (*PM_TraceLine)(Vec3& start, Vec3& end, i32 flags, i32 usehull, i32 ignore_pe);

	Model* (*CL_LoadModel)(const char* modelname, i32* index);
	i32 (*CL_CreateVisibleEntity)(i32 type, Entity* ent);

	const Model* (*GetSpritePointer)(HSprite HSprite);
	void (*pfnPlaySoundByNameAtLocation)(char* sound, f32 volume, Vec3& origin);

	u16 (*pfnPrecacheEvent)(i32 type, const char* psz);
	void (*pfnPlaybackEvent)(i32 flags, const EDict* invoker, u32 eventIndex, f32 delay, Vec3& origin, QAngle& angles, f32 fparam1, f32 fparam2, i32 iparam1, i32 iparam2, i32 bparam1, i32 bparam2);
	void (*pfnWeaponAnim)(i32 iAnim, i32 body);
	f32 (*pfnRandomf32)(f32 flLow, f32 flHigh);
	i32 (*pfnRandomLong)(i32 lLow, i32 lHigh); // long is 4byte owith m32
	void (*pfnHookEvent)(char* name, void (*pfnEvent)(EventArgs* args));
	i32 (*Con_IsVisible)();
	const char* (*pfnGetGameDirectory)(void);
	CVar* (*pfnGetCvarPoi32er)(const char* szName);
	const char* (*Key_LookupBinding)(const char* pBinding);
	const char* (*pfnGetLevelName)(void);
	void (*pfnGetScreenFade)(ScreenFade* fade);
	void (*pfnSetScreenFade)(ScreenFade* fade);
	void* (*VGui_GetPanel)();
	void (*VGui_ViewportPai32Background)(i32 extents[4]);

	u8* (*COM_LoadFile)(char* path, i32 usehunk, i32* pLength);
	char* (*COM_ParseFile)(char* data, char* token);
	void (*COM_FreeFile)(void* buffer);

	//TODO
	struct triangleapi_s* pTriAPI;
	struct efx_api_s* pEfxAPI;
	struct event_api_s* pEventAPI;
	struct demo_api_s* pDemoAPI;
	struct net_api_s* pNetAPI;
	struct IVoiceTweak_s* pVoiceTweak;

	// returns 1 if the client is a spectator only (connected to a proxy), 0 otherwise or 2 if in dev_overview mode
	i32 (*IsSpectateOnly)(void);
	Model* (*LoadMapSprite)(const char* filename);

	// file search functions
	void (*COM_AddAppDirectoryToSearchPath)(const char* baseDir, const char* appName);
	i32 (*COM_ExpandFilename)(const char* fileName, char* nameOutBuffer, i32 nameOutBufferSize);

	// User info
	// playerNum is in the range (1, MaxClients)
	// returns NULL if player doesn't exit
	// returns "" if no value is set
	const char* (*PlayerInfo_ValueForKey)(i32 playerNum, const char* key);
	void (*PlayerInfo_SetValueForKey)(const char* key, const char* value);

	// Gets a unique ID for the specified player. This is the same even if you see the player on a different server.
	// iPlayer is an entity index, so client 0 would use iPlayer=1.
	// Returns false if there is no player on the server in the specified slot.
	QBool (*GetPlayerUniqueID)(i32 iPlayer, char playerID[16]);

	// TrackerID access
	i32 (*GetTrackerIDForPlayer)(i32 playerSlot);
	i32 (*GetPlayerForTrackerID)(i32 trackerID);

	// Same as pfnServerCmd, but the message goes in the unreliable stream so it can't clog the net stream
	// (but it might not get there).
	i32 (*pfnServerCmdUnreliable)(char* szCmdString);

	void (*pfnGetMousePos)(TagPoint* ppt);
	void (*pfnSetMousePos)(i32 x, i32 y);
	void (*pfnSetMouseEnable)(QBool fEnable);
};

extern EngineFuncs* gEngineFuncs;

#endif // ENGINE_ENGINE_HPP
