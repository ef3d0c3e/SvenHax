#ifndef INTERFACE_HPP
#define INTERFACE_HPP

#include "Util/Util.hpp"
#include "Util/VMT.hpp"
#include "SDK/SDK.hpp"
#include "Util/Symbol.hpp"
#include "Engine/ClientDLL.hpp"
#include "Engine/Engine.hpp"
#include "Engine/EngineExportedFuncs.hpp"

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

extern CEngine* engine;

extern VMT* studioRendererVMT;

namespace Interface
{
	void FindSymbols();
	void FindInterfaces();
	void DumpInterfaces();

	void FindClientDLLFuncs();

	void FindGlobals();

	void HookVMs();
}

#endif // INTERFACE_HPP
