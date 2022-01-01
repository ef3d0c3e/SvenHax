#include "Interface.hpp"
#include "Hooks/Hooks.hpp"

#include <sys/types.h>
#include <unistd.h>
#include <dlfcn.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sstream>
#include <link.h>
#include <set>
#include <fstream>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fmt/format.h>


using namespace std::literals;

SymbolTable symbols;

CBaseFileSystem* fileSystem = nullptr;
CDedicatedServerAPI* dedicatedServer = nullptr;
CEngineAPI* engineAPI = nullptr;
CGameUIFuncs* gameUIFuncs = nullptr;
CEngineClient* client = nullptr;
CEngineServer* server = nullptr;
CServerRemoteAccess* remoteAccess = nullptr;
CBaseUI* baseUI = nullptr;
EngineSurface* engineSurface = nullptr;
CSurface* surface = nullptr;
CEngineVGui* engineVGui = nullptr;
CCLientDLL* clientDLL = nullptr;
CSchemeManager* schemeManager = nullptr;
CVGuiKeyValues* keyValues = nullptr;
VPanelWrapper* panel = nullptr;
CVGui* vgui = nullptr;
CInputWin* input = nullptr;
CSystem* csystem = nullptr;
CCareerGame* careerUI = nullptr;
CGameConsole* console = nullptr;
MusicManager* musicManager = nullptr;
CRunGameEngine* gameEngine = nullptr;
CVGuiSystemModuleLoader* moduleLoader = nullptr;
CServerBrowser* serverBrowser = nullptr;
CDefaultCvar* cvar = nullptr;

CEngine* engine = nullptr;
CBasePlayer* gPlayerList = nullptr;

ClientDLLFuncs* gClientDllFuncs = nullptr;
EngineFuncs* gEngineFuncs = nullptr;
EngineExportedFuncs* gExportedFuncs = nullptr;
EngineStudioApi* gStudioApi = nullptr;
PlayerMove* gPMove = nullptr;
std::array<TempEnt, 2048>* gTempEnts = nullptr;
std::array<EDict, 257>* gEnt = nullptr;

CStudioModelRenderer* gStudioRenderer = nullptr;

VMT* studioRendererVMT = nullptr;

void Interface::FindSymbols()
{
	std::map<std::string, std::deque<Maps::MapEntry>> maps;

	Maps::ParseMaps([&maps](Maps::MapEntry&& ent)
	{
		if (ent.path.empty())
			return;
		const auto it = maps.find(ent.path);
		if (it == maps.end())
			maps.insert({ent.path, std::deque<Maps::MapEntry>{ent}});
		else
			it->second.push_back(std::move(ent));
	});

	for (const auto& library : maps)
	{
		auto getName = [](const std::string& path) // Get a readable name
		{
			const auto pos = path.find("Sven Co-op/"); // Path relative to current directory
			if (pos == std::string::npos)
				return path;

			return path.substr(pos + 11);
		};

		const auto name = getName(library.first);
		if (name.find(".so") == std::string::npos)
			continue;
		
		static constexpr std::array<std::string_view, 12> wantedLibraries // c++20 constexpr std::string when
		{
			"steamclient.so"sv,
			"filesystem_stdio.so"sv,
			"hw.so"sv,
			"libiconv.so.2"sv,
			"libsteam_api.so"sv,
			"libtier0.so"sv,
			"libvstdlib.so"sv,
			"serverbrowser_linux.so"sv,
			"client.so"sv,
			"gameui.so"sv,
			"vgui.so"sv,
			"vgui2.so"sv,
		};

		for (const auto& wanted : wantedLibraries)
		{
			if (name.size() < wanted.size() || std::strncmp(&name.c_str()[name.size()-wanted.size()], std::string(wanted).c_str(), wanted.size()) != 0)
				continue;

			int fd;
			fd = open(library.first.c_str(), O_RDONLY);
			struct stat st;
			if (fstat(fd, &st) < 0)
				throw Exception("Interface::FindSymbols() stat failed on '{}'", library.first);


			void* mem = mmap(NULL, st.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
			if (mem == MAP_FAILED)
				throw Exception("Interface::FindSymbols() mmap failed");

			std::deque<E::Symbol> symbolList;
			E::GetSymbols((std::uintptr_t)mem, [&symbolList](E::Symbol&& s)
			{
				symbolList.push_back(std::move(s));
			});
			std::deque<E::Segment> segs;
			E::GetSegments((std::uintptr_t)mem, [&segs](E::Segment&& s)
			{
				segs.push_back(std::move(s));
			});

			if (symbolList.empty())
			{
				munmap(mem, st.st_size);
				break;
			}

			symbols.m_table.insert({name, Library(name, segs, symbolList, library.second)});

			munmap(mem, st.st_size);
			break;
		}
	}
}

void Interface::FindInterfaces()
{
	auto get = []<class T>(std::string name, const char* filename, const char* version, bool exact = false)
	{
		T* p = GetInterface<T>(filename, version, exact);
		if (p == nullptr)
			throw Exception("Interface::FindInterfaces() GetInterface<{0}> returned nullptr", type_name<T>());
		//fmt::print(" * {0} = {1:p}\n", name, (void*)p);
		return p;
	};

	fileSystem      = get.template operator()<CBaseFileSystem>("fileSystem", "filesystem_stdio.so", "VFileSystem");

	dedicatedServer = get.template operator()<CDedicatedServerAPI>("dedicatedServer", "hw.so", "VENGINE_HLDS_API_VERSION");
	engineAPI       = get.template operator()<CEngineAPI>         ("engineAPI",       "hw.so", "VENGINE_LAUNCHER_API_VERSION");
	gameUIFuncs     = get.template operator()<CGameUIFuncs>       ("gameUIFuncs",     "hw.so", "VENGINE_GAMEUIFUNCS_VERSION");
	client          = get.template operator()<CEngineClient>      ("client",          "hw.so", "SCEngineClient");
	server          = get.template operator()<CEngineServer>      ("server",          "hw.so", "SCEngineServer");
	remoteAccess    = get.template operator()<CServerRemoteAccess>("remoteAccess",    "hw.so", "GameServerData");
	baseUI          = get.template operator()<CBaseUI>            ("baseUI",          "hw.so", "BaseUI");
	engineSurface   = get.template operator()<EngineSurface>      ("engineSurface",   "hw.so", "EngineSurface");
	surface         = get.template operator()<CSurface>           ("surface",         "hw.so", "VGUI_Surface");
	engineVGui      = get.template operator()<CEngineVGui>        ("engineVGui",      "hw.so", "VEngineVGui");

	clientDLL       = get.template operator()<CCLientDLL>("clientDLL",      "svencoop/cl_dlls/client.so", "SCClientDLL");

	schemeManager   = get.template operator()<CSchemeManager>("schemeManager", "vgui2.so", "VGUI_Scheme");
	keyValues       = get.template operator()<CVGuiKeyValues>("keyValues",     "vgui2.so", "KeyValues");
	panel           = get.template operator()<VPanelWrapper> ("panel",         "vgui2.so", "VGUI_Panel");
	vgui            = get.template operator()<CVGui>         ("vgui",          "vgui2.so", "VGUI_ivgui");

	input           = get.template operator()<CInputWin>("input",   "vgui2.so", "VGUI_Input");
	csystem         = get.template operator()<CSystem>  ("csystem", "vgui2.so", "VGUI_System");

	careerUI        = get.template operator()<CCareerGame>            ("careerUI",     "svencoop/cl_dlls/gameui.so", "CareerUI");
	console         = get.template operator()<CGameConsole>           ("console",      "svencoop/cl_dlls/gameui.so", "GameConsole");
	musicManager    = get.template operator()<MusicManager>           ("musicManager", "svencoop/cl_dlls/gameui.so", "MusicManager");
	gameEngine      = get.template operator()<CRunGameEngine>         ("gameEngine",   "svencoop/cl_dlls/gameui.so", "RunGameEngine");
	moduleLoader    = get.template operator()<CVGuiSystemModuleLoader>("moduleLoader", "svencoop/cl_dlls/gameui.so", "VGuiModuleLoader");

	serverBrowser   = get.template operator()<CServerBrowser>("serverBrowser", "platform/servers/serverbrowser_linux.so", "ServerBrowser");
	cvar            = get.template operator()<CDefaultCvar>  ("cvar",          "platform/servers/serverbrowser_linux.so", "VEngineCvar");

}

void Interface::DumpInterfaces()
{
	std::stringstream ss;

	std::vector<const char*> modules;

	dl_iterate_phdr([](struct dl_phdr_info* info, size_t size, void* data) {
		reinterpret_cast<std::vector<const char*>*>(data)->push_back(info->dlpi_name);
		return 0;
	}, &modules);

	for (auto module : modules)
	{
		void* library = dlopen(module, RTLD_NOLOAD | RTLD_NOW | RTLD_LOCAL);

		if (library == nullptr)
			continue;

		void* interfaces_sym = dlsym(library, "_ZN12InterfaceReg16s_pInterfaceRegsE"); // gcc mangling

		dlclose(library);
		if (interfaces_sym == nullptr)
			continue;

		InterfaceReg* interfaces = *reinterpret_cast<InterfaceReg**>(interfaces_sym);

		InterfaceReg* cur_interface;

		std::set<const char*> interface_name;

		for (cur_interface = interfaces; cur_interface != nullptr; cur_interface = cur_interface->m_pNext)
		{
			//fmt::print("{0} - {1:p}\n", cur_interface->m_pName, reinterpret_cast<void*>(&cur_interface->m_CreateFn));
			interface_name.insert(cur_interface->m_pName);
		}

		if (interface_name.empty())
			continue;

		ss << module << '\n';

		for (auto interface : interface_name)
			ss << '\t' << interface << '\n';

		ss << '\n';
	}

	std::ofstream("/tmp/svenhaxInterfaces") << ss.str();
}

void Interface::FindClientDLLFuncs()
{
	const auto hw = symbols["hw.so"s];
	gClientDllFuncs = new ClientDLLFuncs
	{
	.Init = reinterpret_cast<ClientDLLFuncs::InitFn>(hw["ClientDLL_Init()"s]),
	.GetUserEntity = reinterpret_cast<ClientDLLFuncs::GetUserEntityFn>(hw["ClientDLL_GetUserEntity(int)"s]),
	.VoiceStatus = reinterpret_cast<ClientDLLFuncs::VoiceStatusFn>(hw["ClientDLL_VoiceStatus(int, int)"s]),
	.ChatInputPosition = reinterpret_cast<ClientDLLFuncs::ChatInputPositionFn>(hw["ClientDLL_ChatInputPosition(int*, int*)"s]),
	.TempEntUpdate = reinterpret_cast<ClientDLLFuncs::TempEntUpdateFn>(hw["ClientDLL_TempEntUpdate(double, double, double, tempent_s**, tempent_s**, int (*)(cl_entity_s*), void (*)(tempent_s*, float))"s]),
	.DirectorMessage = reinterpret_cast<ClientDLLFuncs::DirectorMessageFn>(hw["ClientDLL_DirectorMessage(int, void*)"s]),
	.KeyEvent = reinterpret_cast<ClientDLLFuncs::KeyEventFn>(hw["ClientDLL_Key_Event(int, int, char const*)"s]),
	.GetHullBounds = reinterpret_cast<ClientDLLFuncs::GetHullBoundsFn>(hw["ClientDLL_GetHullBounds(int, float*, float*)"s]),
	.ConnectionLessPacket = reinterpret_cast<ClientDLLFuncs::ConnectionLessPacketFn>(hw["ClientDLL_ConnectionlessPacket(netadr_s const*, char const*, char*, int*)"s]),
	.PostRunCmd = reinterpret_cast<ClientDLLFuncs::PostRunCmdFn>(hw["ClientDLL_PostRunCmd(local_state_s*, local_state_s*, usercmd_s*, int, double, unsigned int)"s]),
	.StudioEvent = reinterpret_cast<ClientDLLFuncs::StudioEventFn>(hw["ClientDLL_StudioEvent(mstudioevent_s const*, cl_entity_s const*)"s]),
	.DrawNormalTriangles = reinterpret_cast<ClientDLLFuncs::DrawNormalTrianglesFn>(hw["ClientDLL_DrawNormalTriangles()"s]),
	.DrawTransparentTriangles = reinterpret_cast<ClientDLLFuncs::DrawTransparentTrianglesFn>(hw["ClientDLL_DrawTransparentTriangles()"s]),
	.CreateEntities = reinterpret_cast<ClientDLLFuncs::CreateEntitiesFn>(hw["ClientDLL_CreateEntities()"s]),
	.AddEntity = reinterpret_cast<ClientDLLFuncs::AddEntityFn>(hw["ClientDLL_AddEntity(int, cl_entity_s*)"s]),
	.CalcRefDef = reinterpret_cast<ClientDLLFuncs::CalcRefDefFn>(hw["ClientDLL_CalcRefdef(ref_params_s*)"s]),
	.CamThink = reinterpret_cast<ClientDLLFuncs::CamThinkFn>(hw["ClientDLL_CAM_Think()"s]),
	.FindKey = reinterpret_cast<ClientDLLFuncs::FindKeyFn>(hw["ClientDLL_FindKey(char const*)"s]),
	.GetCameraOffsets = reinterpret_cast<ClientDLLFuncs::GetCameraOffsetsFn>(hw["ClientDLL_GetCameraOffsets(float*)"s]),
	.IsThirdPerson = reinterpret_cast<ClientDLLFuncs::IsThirdPersonFn>(hw["ClientDLL_IsThirdPerson()"s]),
	.CreateMove = reinterpret_cast<ClientDLLFuncs::CreateMoveFn>(hw["ClientDLL_CreateMove(float, usercmd_s*, int)"s]),
	.ActivateMouse = reinterpret_cast<ClientDLLFuncs::ActivateMouseFn>(hw["ClientDLL_ActivateMouse()"s]),
	.DeactivateMouse = reinterpret_cast<ClientDLLFuncs::DeactivateMouseFn>(hw["ClientDLL_DeactivateMouse()"s]),
	.MouseEvent = reinterpret_cast<ClientDLLFuncs::MouseEventFn>(hw["ClientDLL_MouseEvent(int)"s]),
	.ClearStates = reinterpret_cast<ClientDLLFuncs::ClearStatesFn>(hw["ClientDLL_ClearStates()"s]),
	.Accumulate = reinterpret_cast<ClientDLLFuncs::AccumulateFn>(hw["ClientDLL_IN_Accumulate()"s]),
	.ClientMoveInit = reinterpret_cast<ClientDLLFuncs::ClientMoveInitFn>(hw["ClientDLL_ClientMoveInit(playermove_s*)"s]),
	.ClientTextureType = reinterpret_cast<ClientDLLFuncs::ClientTextureTypeFn>(hw["ClientDLL_ClientTextureType(char*)"s]),
	.ClientMove = reinterpret_cast<ClientDLLFuncs::ClientMoveFn>(hw["ClientDLL_MoveClient(playermove_s*)"s]),
	.Shutdown = reinterpret_cast<ClientDLLFuncs::ShutdownFn>(hw["ClientDLL_Shutdown()"s]),
	.HudVidInit = reinterpret_cast<ClientDLLFuncs::HudVidInitFn>(hw["ClientDLL_HudVidInit()"s]),
	//ClientDLL_HudInit()
	.HudRedraw = reinterpret_cast<ClientDLLFuncs::HudRedrawFn>(hw["ClientDLL_HudRedraw(int)"s]),
	.Frame = reinterpret_cast<ClientDLLFuncs::FrameFn>(hw["ClientDLL_Frame(double)"s]),
	.UpdateClientData = reinterpret_cast<ClientDLLFuncs::UpdateClientDataFn>(hw["ClientDLL_UpdateClientData()"s]),
	//ClientDLL_DemoUpdateClientData(client_data_s*)
	.TxferLocalOverrides = reinterpret_cast<ClientDLLFuncs::TxferLocalOverridesFn>(hw["ClientDLL_TxferLocalOverrides(entity_state_s*, clientdata_s const*)"s]),
	.ProcessPlayerState = reinterpret_cast<ClientDLLFuncs::ProcessPlayerStateFn>(hw["ClientDLL_ProcessPlayerState(entity_state_s*, entity_state_s const*)"s]),
	.TxferPredictionData = reinterpret_cast<ClientDLLFuncs::TxferPredictionDataFn>(hw["ClientDLL_TxferPredictionData(entity_state_s*, entity_state_s const*, clientdata_s*, clientdata_s const*, weapon_data_s*, weapon_data_s const*)"s]),
	.ReadDemoBuffer = reinterpret_cast<ClientDLLFuncs::ReadDemoBufferFn>(hw["ClientDLL_ReadDemoBuffer(int, unsigned char*)"s]),
	//ClientDLL_CheckStudioInterface(void*)
	};
}

void Interface::FindGlobals()
{
	gEngineFuncs = reinterpret_cast<EngineFuncs*>(symbols["svencoop/cl_dlls/client.so"s]["gEngfuncs"s]);
	gExportedFuncs = reinterpret_cast<EngineExportedFuncs*>(symbols["hw.so"s]["g_engfuncsExportedToDlls"s]);

	gStudioApi = reinterpret_cast<EngineStudioApi*>(symbols["svencoop/cl_dlls/client.so"s]["IEngineStudio"s]);

	engine = reinterpret_cast<CEngine*>(symbols["hw.so"s]["g_Engine"s]);

	gPlayerList = reinterpret_cast<CBasePlayer*>(symbols["svencoop/cl_dlls/client.so"s]["player"s]);

	gPMove = reinterpret_cast<PlayerMove*>(symbols["hw.so"s]["g_clmove"s]);

	gTempEnts = reinterpret_cast<std::array<TempEnt, 2048>*>(symbols["hw.so"s]["gTempEnts"s]);

	gEnt = reinterpret_cast<std::array<EDict, 257>*>(symbols["svencoop/cl_dlls/client.so"s]["ent"s]);

	gStudioRenderer = reinterpret_cast<CStudioModelRenderer*>(symbols["svencoop/cl_dlls/client.so"s]["g_StudioRenderer"s]);
}

void Interface::HookVMs()
{
	studioRendererVMT = new VMT(gStudioRenderer);
	studioRendererVMT->HookVM(StudioModelRenderer::StudioDrawModel, 3);
	studioRendererVMT->HookVM(StudioModelRenderer::StudioDrawPlayer, 4);
	studioRendererVMT->HookVM(StudioModelRenderer::StudioDrawMonster, 5);
	studioRendererVMT->ApplyVMT();
}
