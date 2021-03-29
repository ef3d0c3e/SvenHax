#ifndef SDK_GLOBALS_HPP
#define SDK_GLOBALS_HPP

#include "IBaseInterface.hpp"
#include "EngineSurface.hpp" // qboolean
#include "Common.hpp"


struct EngineFuncs;

typedef int (*InitializeFn)(EngineFuncs*, int);
typedef void (*InitFn)(void);
typedef int (*VidInitFn)(void);
typedef int (*RedrawFn)(float, int);
typedef int (*UpdateClientDataFn)(ClientData*, float);
typedef void (*ResetFn)(void);
typedef void (*ClientMoveFn)(struct playermove_s* ppmove, qboolean server);
typedef void (*ClientMoveInitFn)(struct playermove_s* ppmove);
typedef char (*ClientMoveTextureFn)(char* name);
typedef void (*InActivateMouseFn)(void);
typedef void (*InDeactivateMouseFn)(void);
typedef void (*InMouseEventFn)(int mstate);
typedef void (*InClearStatesFn)(void);
typedef void (*InAccumulateFn)(void);
typedef void (*ClCreateMoveFn)(float frametime, UserCmd* cmd, int active);
typedef int (*ClIsThirdPersonFn)(void);
typedef void (*ClGetCameraOffsetsFn)(float* ofs);
typedef struct kbutton_s* (*KbFindFn)(const char* name);
typedef void (*CamThinkFn)(void);
typedef void (*CalcRefDefFn)(struct ref_params_s* pparams);
typedef int (*AddEntityFn)(int type, struct cl_entity_s* ent, const char* modelname);
typedef void (*CreateEntitiesFn)(void);
typedef void (*DrawNormalTrianglesFn)(void);
typedef void (*DrawTransparentTrianglesFn)(void);
typedef void (*StudioEventFn)(const struct mstudioevent_s* event, const struct cl_entity_s* entity);
typedef void (*PostRunCmdFn)(struct local_state_s* from, struct local_state_s* to, UserCmd* cmd, int runfuncs, double time, unsigned int random_seed);
typedef void (*ShutdownFn)(void);
typedef void (*TxferLocalOverridesFn)(struct entity_state_s* state, const struct clientdata_s* client);
typedef void (*ProcessPlayerStateFn)(struct entity_state_s* dst, const struct entity_state_s* src);
typedef void (*TxferPredictionDataFn)(struct entity_state_s* ps, const struct entity_state_s* pps, struct clientdata_s* pcd, const struct clientdata_s* ppcd, struct weapon_data_s* wd, const struct weapon_data_s* pwd);
typedef void (*DemoReadBufferFn)(int size, unsigned char* buffer);
typedef int (*ConnectionLessPacketFn)(const struct netadr_s* net_from, const char* args, char* response_buffer, int* response_buffer_size);
typedef int (*GetHullBoundsFn)(int hullnumber, float* mins, float* maxs);
typedef void (*FrameFn)(double);
typedef int (*KeyEventFn)(int eventcode, int keynum, const char* pszCurrentBinding);
typedef void (*TempEntUpdateFn)(double frametime, double client_time, double cl_gravity, struct tempent_s** ppTempEntFree, struct tempent_s** ppTempEntActive, int (*Callback_AddVisibleEntity)(struct cl_entity_s* pEntity), void (*Callback_TempEntPlaySound)(struct tempent_s* pTemp, float damp));
typedef struct cl_entity_s* (*GetUserEntityFn)(int index);
typedef void (*VoiceStatusFn)(int entindex, qboolean bTalking);
typedef void (*DirectorMessageFn)(int iSize, void* pbuf);
typedef int (*GetStudioModelInterfaceFn)(int version, struct r_studio_interface_s** ppinterface, struct engine_studio_api_s* pstudio);
typedef void (*ChatInputPositionFn)(int* x, int* y);
typedef int (*GetPlayerTeamFn)(int iplayer);

struct EngineFuncs
{
	InitializeFn Initialize;
	InitFn Init;
	VidInitFn VidInit;
	RedrawFn Redraw;
	UpdateClientDataFn UpdateClientData;
	ResetFn Reset;
	ClientMoveFn ClientMove;
	ClientMoveInitFn ClientMoveInit;
	ClientMoveTextureFn ClientMoveTexture;
	InActivateMouseFn InActivateMouse;
	InDeactivateMouseFn InDeactivateMouse;
	InMouseEventFn InMouseEvent;
	InClearStatesFn InClearStates;
	InAccumulateFn InAccumulate;
	ClCreateMoveFn ClCreateMove;
	ClIsThirdPersonFn ClIsThirdPerson;
	ClGetCameraOffsetsFn ClGetCameraOffsets;
	KbFindFn KbFind;
	CamThinkFn CamThink;
	CalcRefDefFn CalcRefDef;
	AddEntityFn AddEntity;
	CreateEntitiesFn CreateEntities;
	DrawNormalTrianglesFn DrawNormalTriangles;
	DrawTransparentTrianglesFn DrawTransparentTriangles;
	StudioEventFn StudioEvent;
	PostRunCmdFn PostRunCmd;
	ShutdownFn Shutdown;
	TxferLocalOverridesFn TxferLocalOverrides;
	ProcessPlayerStateFn ProcessPlayerState;
	TxferPredictionDataFn TxferPredictionData;
	DemoReadBufferFn DemoReadBuffer;
	ConnectionLessPacketFn ConnectionLessPacket;
	GetHullBoundsFn GetHullBounds;
	FrameFn Frame;
	KeyEventFn KeyEvent;
	TempEntUpdateFn TempEntUpdate;
	GetUserEntityFn GetUserEntity;
	VoiceStatusFn VoiceStatus;
	DirectorMessageFn DirectorMessage;
	GetStudioModelInterfaceFn GetStudioModelInterface;
	ChatInputPositionFn ChatInputPosition;
	GetPlayerTeamFn GetPlayerTeam;
	CreateInterfaceFn ClientFactory;
};

#endif // SDK_GLOBALS_HPP
