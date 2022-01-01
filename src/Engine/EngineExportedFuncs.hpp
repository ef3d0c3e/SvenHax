#ifndef ENGINE_ENGINEEXPORTEDFUNCS_HPP
#define ENGINE_ENGINEEXPORTEDFUNCS_HPP

#include "Primitives/CVar.hpp"
#include "Primitives/EDict.hpp"


MAKE_CENUM_Q(AlertType, i32,
	NOTICE, 0,
	CONSOLE, 1,
	AICONSOLE, 2,
	WARNING, 3,
	ERROR, 4,
	LOGGED, 5);

MAKE_CENUM_Q(PrintType, i32,
	CONSOLE, 0,
	CENTER, 1,
	CHAT, 2);

MAKE_CENUM_Q(ForceType, i32,
	EXACTFILE, 0,
	MODEL_SAMEBOUNDS, 1,
	MODEL_SPECIFYBOUNDS, 2,
	MODEL_SPECIFYBOUNDS_IF_AVAIL, 3);


struct TraceResult;
struct sentenceEntry_s;
struct sequenceEntry_s;
struct delta_s;
struct EngineExportedFuncs
{
	int			(*pfnPrecacheModel)			(char* s);
	int			(*pfnPrecacheSound)			(char* s);
	void		(*pfnSetModel)				(EDict *e, const char *m);
	int			(*pfnModelIndex)			(const char *m);
	int			(*pfnModelFrames)			(int modelIndex);
	void		(*pfnSetSize)				(EDict *e, const f32 *rgflMin, const f32 *rgflMax);
	void		(*pfnChangeLevel)			(char* s1, char* s2);
	void		(*pfnGetSpawnParms)			(EDict *ent);
	void		(*pfnSaveSpawnParms)		(EDict *ent);
	f32		(*pfnVecToYaw)				(const f32 *rgflVector);
	void		(*pfnVecToAngles)			(const f32 *rgflVectorIn, f32 *rgflVectorOut);
	void		(*pfnMoveToOrigin)			(EDict *ent, const f32 *pflGoal, f32 dist, int iMoveType);
	void		(*pfnChangeYaw)				(EDict* ent);
	void		(*pfnChangePitch)			(EDict* ent);
	EDict*	(*pfnFindEntityByString)	(EDict *pEdictStartSearchAfter, const char *pszField, const char *pszValue);
	int			(*pfnGetEntityIllum)		(EDict* pEnt);
	EDict*	(*pfnFindEntityInSphere)	(EDict *pEdictStartSearchAfter, const f32 *org, f32 rad);
	EDict*	(*pfnFindClientInPVS)		(EDict *pEdict);
	EDict* (*pfnEntitiesInPVS)			(EDict *pplayer);
	void		(*pfnMakeVectors)			(const f32 *rgflVector);
	void		(*pfnAngleVectors)			(const f32 *rgflVector, f32 *forward, f32 *right, f32 *up);
	EDict*	(*pfnCreateEntity)			(void);
	void		(*pfnRemoveEntity)			(EDict* e);
	EDict*	(*pfnCreateNamedEntity)		(StringT className);
	void		(*pfnMakeStatic)			(EDict *ent);
	int			(*pfnEntIsOnFloor)			(EDict *e);
	int			(*pfnDropToFloor)			(EDict* e);
	int			(*pfnWalkMove)				(EDict *ent, f32 yaw, f32 dist, int iMode);
	void		(*pfnSetOrigin)				(EDict *e, const f32 *rgflOrigin);
	void		(*pfnEmitSound)				(EDict *entity, int channel, const char *sample, /*int*/f32 volume, f32 attenuation, int fFlags, int pitch);
	void		(*pfnEmitAmbientSound)		(EDict *entity, f32 *pos, const char *samp, f32 vol, f32 attenuation, int fFlags, int pitch);
	void		(*pfnTraceLine)				(const f32 *v1, const f32 *v2, int fNoMonsters, EDict *pentToSkip, TraceResult *ptr);
	void		(*pfnTraceToss)				(EDict* pent, EDict* pentToIgnore, TraceResult *ptr);
	int			(*pfnTraceMonsterHull)		(EDict *pEdict, const f32 *v1, const f32 *v2, int fNoMonsters, EDict *pentToSkip, TraceResult *ptr);
	void		(*pfnTraceHull)				(const f32 *v1, const f32 *v2, int fNoMonsters, int hullNumber, EDict *pentToSkip, TraceResult *ptr);
	void		(*pfnTraceModel)			(const f32 *v1, const f32 *v2, int hullNumber, EDict *pent, TraceResult *ptr);
	const char *(*pfnTraceTexture)			(EDict *pTextureEntity, const f32 *v1, const f32 *v2 );
	void		(*pfnTraceSphere)			(const f32 *v1, const f32 *v2, int fNoMonsters, f32 radius, EDict *pentToSkip, TraceResult *ptr);
	void		(*pfnGetAimVector)			(EDict* ent, f32 speed, f32 *rgflReturn);
	void		(*pfnServerCommand)			(char* str);
	void		(*pfnServerExecute)			(void);
	void		(*pfnClientCommand)			(EDict* pEdict, char* szFmt, ...);
	void		(*pfnParticleEffect)		(const f32 *org, const f32 *dir, f32 color, f32 count);
	void		(*pfnLightStyle)			(int style, char* val);
	int			(*pfnDecalIndex)			(const char *name);
	int			(*pfnPointContents)			(const f32 *rgflVector);
	void		(*pfnMessageBegin)			(int msg_dest, int msg_type, const f32 *pOrigin, EDict *ed);
	void		(*pfnMessageEnd)			(void);
	void		(*pfnWriteByte)				(int iValue);
	void		(*pfnWriteChar)				(int iValue);
	void		(*pfnWriteShort)			(int iValue);
	void		(*pfnWriteLong)				(int iValue);
	void		(*pfnWriteAngle)			(f32 flValue);
	void		(*pfnWriteCoord)			(f32 flValue);
	void		(*pfnWriteString)			(const char *sz);
	void		(*pfnWriteEntity)			(int iValue);
	void		(*pfnCVarRegister)			(CVar* pCvar);
	f32		(*pfnCVarGetFloat)			(const char *szVarName);
	const char*	(*pfnCVarGetString)			(const char *szVarName);
	void		(*pfnCVarSetFloat)			(const char *szVarName, f32 flValue);
	void		(*pfnCVarSetString)			(const char *szVarName, const char *szValue);
	void		(*pfnAlertMessage)			(AlertType atype, char *szFmt, ...);
	void		(*pfnEngineFprintf)			(void *pfile, char *szFmt, ...);
	void*		(*pfnPvAllocEntPrivateData)	(EDict *pEdict, i32 cb);
	void*		(*pfnPvEntPrivateData)		(EDict *pEdict);
	void		(*pfnFreeEntPrivateData)	(EDict *pEdict);
	const char*	(*StringFromIndex)			(StringT iString);
	int			(*pfnAllocString)			(const char *szValue);
	EntVars*	(*pfnGetVarsOfEnt)			(EDict *pEdict);
	EDict*	(*pfnPEntityOfEntOffset)	(int iEntOffset);
	int			(*pfnEntOffsetOfPEntity)	(const EDict *pEdict);
	int			(*pfnIndexOfEdict)			(const EDict *pEdict);
	EDict*	(*EntityOfEntIndex)		(int iEntIndex);
	EDict*	(*pfnFindEntityByVars)		(struct entvars_s* pvars);
	void*		(*pfnGetModelPtr)			(EDict* pEdict);
	int			(*pfnRegUserMsg)			(const char *pszName, int iSize);
	void		(*pfnAnimationAutomove)		(const EDict* pEdict, f32 flTime);
	void		(*pfnGetBonePosition)		(const EDict* pEdict, int iBone, f32 *rgflOrigin, f32 *rgflAngles );
	u32 (*pfnFunctionFromName)	( const char *pName );
	const char *(*pfnNameForFunction)		( u32 function );
	void		(*pfnClientPrintf)			( EDict* pEdict, PrintType ptype, const char *szMsg ); // JOHN: engine callbacks so game DLL can print messages to individual clients
	void		(*pfnServerPrint)			( const char *szMsg );
	const char *(*pfnCmd_Args)				( void );		// these 3 added 
	const char *(*pfnCmd_Argv)				( int argc );	// so game DLL can easily 
	int			(*pfnCmd_Argc)				( void );		// access client 'cmd' strings
	void		(*pfnGetAttachment)			(const EDict *pEdict, int iAttachment, f32 *rgflOrigin, f32 *rgflAngles );
	void		(*pfnCRC32_Init)			(CRC32 *pulCRC);
	void        (*pfnCRC32_ProcessBuffer)   (CRC32 *pulCRC, void *p, int len);
	void		(*pfnCRC32_ProcessByte)     (CRC32 *pulCRC, unsigned char ch);
	CRC32		(*pfnCRC32_Final)			(CRC32 pulCRC);
	i32		(*pfnRandomLong)			(i32  lLow,  i32  lHigh);
	f32		(*pfnRandomFloat)			(f32 flLow, f32 flHigh);
	void		(*pfnSetView)				(const EDict *pClient, const EDict *pViewent );
	f32		(*pfnTime)					( void );
	void		(*pfnCrosshairAngle)		(const EDict *pClient, f32 pitch, f32 yaw);
	u8*      (*pfnLoadFileForMe)         (char *filename, int *pLength);
	void        (*pfnFreeFile)              (void *buffer);
	void        (*pfnEndSection)            (const char *pszSectionName); // trigger_endsection
	int 		(*pfnCompareFileTime)       (char *filename1, char *filename2, int *iCompare);
	void        (*pfnGetGameDir)            (char *szGetGameDir);
	void		(*pfnCvar_RegisterVariable) (CVar *variable);
	void        (*pfnFadeClientVolume)      (const EDict *pEdict, int fadePercent, int fadeOutSeconds, int holdTime, int fadeInSeconds);
	void        (*pfnSetClientMaxspeed)     (const EDict *pEdict, f32 fNewMaxspeed);
	EDict *	(*pfnCreateFakeClient)		(const char *netname);	// returns NULL if fake client can't be created
	void		(*pfnRunPlayerMove)			(EDict *fakeclient, const f32 *viewangles, f32 forwardmove, f32 sidemove, f32 upmove, unsigned short buttons, u8 impulse, u8 msec );
	int			(*pfnNumberOfEntities)		(void);
	char*		(*pfnGetInfoKeyBuffer)		(EDict *e);	// passing in NULL gets the serverinfo
	char*		(*pfnInfoKeyValue)			(char *infobuffer, char *key);
	void		(*pfnSetKeyValue)			(char *infobuffer, char *key, char *value);
	void		(*pfnSetClientKeyValue)		(int clientIndex, char *infobuffer, char *key, char *value);
	int			(*pfnIsMapValid)			(char *filename);
	void		(*pfnStaticDecal)			( const f32 *origin, int decalIndex, int entityIndex, int modelIndex );
	int			(*pfnPrecacheGeneric)		(char* s);
	int			(*pfnGetPlayerUserId)		(EDict *e ); // returns the server assigned userid for this player.  useful for logging frags, etc.  returns -1 if the edict couldn't be found in the list of clients
	void		(*pfnBuildSoundMsg)			(EDict *entity, int channel, const char *sample, /*int*/f32 volume, f32 attenuation, int fFlags, int pitch, int msg_dest, int msg_type, const f32 *pOrigin, EDict *ed);
	int			(*pfnIsDedicatedServer)		(void);// is this a dedicated server?
	CVar		*(*pfnCVarGetPointer)		(const char *szVarName);
	unsigned int (*pfnGetPlayerWONId)		(EDict *e); // returns the server assigned WONid for this player.  useful for logging frags, etc.  returns -1 if the edict couldn't be found in the list of clients

	// YWB 8/1/99 TFF Physics additions
	void		(*pfnInfo_RemoveKey)		( char *s, const char *key );
	const char *(*pfnGetPhysicsKeyValue)	( const EDict *pClient, const char *key );
	void		(*pfnSetPhysicsKeyValue)	( const EDict *pClient, const char *key, const char *value );
	const char *(*pfnGetPhysicsInfoString)	( const EDict *pClient );
	unsigned short (*pfnPrecacheEvent)		( int type, const char*psz );
	void		(*pfnPlaybackEvent)			( int flags, const EDict *pInvoker, unsigned short eventindex, f32 delay, f32 *origin, f32 *angles, f32 fparam1, f32 fparam2, int iparam1, int iparam2, int bparam1, int bparam2 );

	unsigned char *(*pfnSetFatPVS)			( f32 *org );
	unsigned char *(*pfnSetFatPAS)			( f32 *org );

	int			(*pfnCheckVisibility )		( const EDict *entity, unsigned char *pset );

	void		(*pfnDeltaSetField)			( struct delta_s *pFields, const char *fieldname );
	void		(*pfnDeltaUnsetField)		( struct delta_s *pFields, const char *fieldname );
	void		(*pfnDeltaAddEncoder)		( char *name, void (*conditionalencode)( struct delta_s *pFields, const unsigned char *from, const unsigned char *to ) );
	int			(*pfnGetCurrentPlayer)		( void );
	int			(*pfnCanSkipPlayer)			( const EDict *player );
	int			(*pfnDeltaFindField)		( struct delta_s *pFields, const char *fieldname );
	void		(*pfnDeltaSetFieldByIndex)	( struct delta_s *pFields, int fieldNumber );
	void		(*pfnDeltaUnsetFieldByIndex)( struct delta_s *pFields, int fieldNumber );

	void		(*pfnSetGroupMask)			( int mask, int op );

	int			(*pfnCreateInstancedBaseline) ( StringT classname, struct entity_state_s *baseline );
	void		(*pfnCvar_DirectSet)		( struct cvar_s *var, char *value );

	// Forces the client and server to be running with the same version of the specified file
	//  ( e.g., a player model ).
	// Calling this has no effect in single player
	void		(*pfnForceUnmodified)		( ForceType type, f32 *mins, f32 *maxs, const char *filename );

	void		(*pfnGetPlayerStats)		( const EDict *pClient, int *ping, int *packet_loss );

	void		(*pfnAddServerCommand)		( char *cmd_name, void (*function) (void) );

	// For voice communications, set which clients hear eachother.
	// NOTE: these functions take player entity indices (starting at 1).
	QBool	(*pfnVoice_GetClientListening)(int iReceiver, int iSender);
	QBool	(*pfnVoice_SetClientListening)(int iReceiver, int iSender, QBool bListen);

	const char *(*pfnGetPlayerAuthId)		( EDict *e );

	// PSV: Added for CZ training map
//	const char *(*pfnKeyNameForBinding)					( const char* pBinding );
	
	sequenceEntry_s*	(*pfnSequenceGet)				( const char* fileName, const char* entryName );
	sentenceEntry_s*	(*pfnSequencePickSentence)		( const char* groupName, int pickMethod, int *picked );

	// LH: Give access to filesize via filesystem
	int			(*pfnGetFileSize)						( char *filename );

	unsigned int (*pfnGetApproxWavePlayLen)				(const char *filepath);
	// MDC: Added for CZ career-mode
	int			(*pfnIsCareerMatch)						( void );

	// BGC: return the number of characters of the localized string referenced by using "label"
	int			(*pfnGetLocalizedStringLength)			(const char *label);

	// BGC: added to facilitate persistent storage of tutor message decay values for
	// different career game profiles.  Also needs to persist regardless of mp.dll being
	// destroyed and recreated.
	void		(*pfnRegisterTutorMessageShown)			(int mid);
	int			(*pfnGetTimesTutorMessageShown)			(int mid);
	void		(*pfnProcessTutorMessageDecayBuffer)	(int *buffer, int bufferLength);
	void		(*pfnConstructTutorMessageDecayBuffer)	(int *buffer, int bufferLength);
	void		(*pfnResetTutorMessageDecayData)		( void );
	void		(*pfnQueryClientCvarValue)				( const EDict *player, const char *cvarName );
    void        (*pfnQueryClientCvarValue2)             ( const EDict *player, const char *cvarName, int requestID );
};

extern EngineExportedFuncs* gExportedFuncs;

#endif // ENGINE_ENGINEEXPORTEDFUNCS_HPP
