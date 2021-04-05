#ifndef INTERFACE_HPP
#define INTERFACE_HPP

#include "Util/Util.hpp"
#include "Util/VMT.hpp"
#include "SDK/SDK.hpp"
#include "Util/Symbol.hpp"

extern CBaseFileSystem* fileSystem;
extern CDedicatedServerAPI* dedicatedServer;
extern CEngineAPI* engineAPI;
extern CGameUIFuncs* gameUIFuncs;
extern CEngineClient* client;
extern CEngineServer* server;
extern CServerRemoteAccess* remoteAccess;
extern CBaseUI* baseUI;
extern EngineSurface* engineSurface;
extern CSurface* surface;
extern CEngineVGui* engineVGui;
extern CCLientDLL* clientDLL;
extern CSchemeManager* schemeManager;
extern CVGuiKeyValues* keyValues;
extern VPanelWrapper* panel;
extern CVGui* vgui;
extern CInputWin* input;
extern CSystem* csystem;
extern CCareerGame* careerUI;
extern CGameConsole* console;
extern MusicManager* musicManager;
extern CRunGameEngine* gameEngine;
extern CVGuiSystemModuleLoader* moduleLoader;
extern CServerBrowser* serverBrowser;
extern CDefaultCvar* cvar;

typedef i32 (*GetLocalPlayerFn)(void);
extern GetLocalPlayerFn GetLocalPlayer;

namespace Interface
{
	void FindInterfaces();
	void DumpInterfaces();

	extern std::uintptr_t BaseAddr;
	std::uintptr_t GetBaseAddress();

	void FindClientDLLFuncs();
	void FindFunctions();
}

#endif // INTERFACE_HPP
