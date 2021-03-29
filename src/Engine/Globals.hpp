#ifndef SDK_GLOBALS_HPP
#define SDK_GLOBALS_HPP

#include "Primitives/ClientData.hpp"
#include "PlayerMove.hpp"
#include "Primitives/RefParams.hpp"
#include "Entity.hpp"
#include "Primitives/KButton.hpp"
#include "Primitives/MStudioEvent.hpp"
#include "Primitives/LocalState.hpp"
#include "Primitives/WeaponData.hpp"
#include "Primitives/EntityState.hpp"
#include "Primitives/NetAdr.hpp"
#include "Primitives/TempEnt.hpp"
#include "Primitives/StudioInterface.hpp"
#include "EngineStudioApi.hpp"


struct EngineFuncs;

typedef i32 (*InitializeFn)
	(EngineFuncs*,
	 i32);
typedef void (*InitFn)
	(void);
typedef i32 (*VidInitFn)
	(void);
typedef i32 (*RedrawFn)
	(f32,
	 i32);
typedef i32 (*UpdateClientDataFn)
	(ClientData*,
	 f32);
typedef void (*ResetFn)
	(void);
typedef void (*ClientMoveFn)
	(PlayerMove* pMove,
	 QBool server);
typedef void (*ClientMoveInitFn)
	(PlayerMove* pMove);
typedef char (*ClientMoveTextureFn)
	(char* name);
typedef void (*InActivateMouseFn)
	(void);
typedef void (*InDeactivateMouseFn)
	(void);
typedef void (*InMouseEventFn)
	(i32 mState);
typedef void (*InClearStatesFn)
	(void);
typedef void (*InAccumulateFn)
	(void);
typedef void (*ClCreateMoveFn)
	(f32 frameTime,
	 UserCmd* cmd,
	 i32 active);
typedef i32 (*ClIsThirdPersonFn)
	(void);
typedef void (*ClGetCameraOffsetsFn)
	(f32* ofs);
typedef KButton* (*KbFindFn)
	(const char* name);
typedef void (*CamThinkFn)
	(void);
typedef void (*CalcRefDefFn)
	(RefParams* pparams);
typedef i32 (*AddEntityFn)
	(i32 type,
	 Entity* ent,
	 const char* modelName);
typedef void (*CreateEntitiesFn)
	(void);
typedef void (*DrawNormalTrianglesFn)
	(void);
typedef void (*DrawTransparentTrianglesFn)
	(void);
typedef void (*StudioEventFn)
	(const MStudioEvent* event,
	 const struct Entity* entity);
typedef void (*PostRunCmdFn)
	(LocalState* from,
	 LocalState* to,
	 UserCmd* cmd,
	 i32 runfuncs,
	 double time,
	 u32 random_seed);
typedef void (*ShutdownFn)
	(void);
typedef void (*TxferLocalOverridesFn)
	(EntityState* state,
	 const ClientData* client);
typedef void (*ProcessPlayerStateFn)
	(EntityState* dst,
	 const EntityState* src);
typedef void (*TxferPredictionDataFn)
	(EntityState* ps,
	 const EntityState* pps,
	 ClientData* pcd,
	 const ClientData* ppcd,
	 WeaponData* wd,
	 const WeaponData* pwd);
typedef void (*DemoReadBufferFn)
	(i32 size,
	 u8* buffer);
typedef i32 (*ConnectionLessPacketFn)
	(const NetAdr* net_from,
	 const char* args,
	 char* response_buffer,
	 i32* response_buffer_size);
typedef i32 (*GetHullBoundsFn)
	(i32 hullnumber,
	 f32* mins,
	 f32* maxs);
typedef void (*FrameFn)
	(double);
typedef i32 (*KeyEventFn)
	(i32 eventCode,
	 i32 keyNum,
	 const char* currentBinding);
typedef void (*TempEntUpdateFn)
	(double frameTime,
	 double clientTime,
	 double gravity,
	 TempEnt** tempEntFree,
	 TempEnt** tempEntActive,
	 i32 (*Callback_AddVisibleEntity)(Entity* entity),
	 void (*Callback_TempEntPlaySound)(TempEnt* temp, f32 damp));
typedef struct Entity* (*GetUserEntityFn)(
	(i32 index);
typedef void (*VoiceStatusFn)
	(i32 entIndex,
	 QBool talking);
typedef void (*DirectorMessageFn)
	(i32 size,
	 void* buf);
typedef i32 (*GetStudioModelInterfaceFn)
	(i32 version,
	 StudioInterface** interface,
	 EngineStudioApi* pstudio);
typedef void (*ChatInputPositionFn)
	(i32* x,
	 i32* y);
typedef i32 (*GetPlayerTeamFn)
	(i32 iplayer);

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
