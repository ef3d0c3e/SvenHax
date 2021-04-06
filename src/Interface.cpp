#include "Interface.hpp"
#include "Engine/ClientDLL.hpp"
#include <fmt/format.h>

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

ClientDLLFuncs* gClientDllFuncs = nullptr;

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
			const std::string s(path);
			const auto pos = s.rfind('/');
			if (pos == std::string::npos)
				return s;

			return s.substr(pos + 1);

			/* Gets the name for dlopen() (somewhat)
			if (strlen(name) < 5) // "/home"
				return name;

			if (strcmp(name, "/home") <= 0)
				return name;
			std::string s(name);

			const auto pos = s.find("Sven Co-op/");
			if (pos == std::string::npos)
				return s;

			return s.substr(pos + 11);*/
		};

		const auto name = getName(library.first);
		if (name.find(".so") == std::string::npos)
			continue;
		
		static constexpr std::array<std::string_view, 14> wantedLibraries // c++20 constexpr std::string when
		{
			"steamclient.so"sv,
			"filesystem_stdio.so"sv,
			"hw.so"sv,
			"libSDL2-2.0.so.0"sv,
			"libdiscord-rpc.so"sv,
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
			if (name.size() < wanted.size() || name.compare(0, wanted.size(), wanted) != 0)
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

			if (symbolList.empty())
			{
				munmap(mem, st.st_size);
				break;
			}

			symbols.m_table.insert({name, Library(name, symbolList, library.second)});

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
		fmt::print(" * {0} = {1:p}\n", name, (void*)p);
		return p;
	};

	fileSystem      = get.template operator()<CBaseFileSystem>        ("fileSystem",      "filesystem_stdio.so",        "VFileSystem");

	dedicatedServer = get.template operator()<CDedicatedServerAPI>    ("dedicatedServer","hw.so",                      "VENGINE_HLDS_API_VERSION");
	engineAPI       = get.template operator()<CEngineAPI>             ("engineAPI",      "hw.so",                      "VENGINE_LAUNCHER_API_VERSION");
	gameUIFuncs     = get.template operator()<CGameUIFuncs>           ("gameUIFuncs",    "hw.so",                      "VENGINE_GAMEUIFUNCS_VERSION");
	client          = get.template operator()<CEngineClient>          ("client",         "hw.so",                      "SCEngineClient");
	server          = get.template operator()<CEngineServer>          ("server",         "hw.so",                      "SCEngineServer");
	remoteAccess    = get.template operator()<CServerRemoteAccess>    ("remoteAccess",   "hw.so",                      "GameServerData");
	baseUI          = get.template operator()<CBaseUI>                ("baseUI",         "hw.so",                      "BaseUI");
	engineSurface   = get.template operator()<EngineSurface>          ("engineSurface",  "hw.so",                      "EngineSurface");
	surface         = get.template operator()<CSurface>               ("surface",        "hw.so",                      "VGUI_Surface");
	engineVGui      = get.template operator()<CEngineVGui>            ("engineVGui",     "hw.so",                      "VEngineVGui");

	clientDLL       = get.template operator()<CCLientDLL>             ("clientDLL",      "svencoop/cl_dlls/client.so", "SCClientDLL");

	schemeManager   = get.template operator()<CSchemeManager>         ("schemeManager",  "vgui2.so",                   "VGUI_Scheme");
	keyValues       = get.template operator()<CVGuiKeyValues>         ("keyValues",      "vgui2.so",                   "KeyValues");
	panel           = get.template operator()<VPanelWrapper>          ("panel",          "vgui2.so",                   "VGUI_Panel");
	vgui            = get.template operator()<CVGui>                  ("vgui",           "vgui2.so",                   "VGUI_ivgui");

	input           = get.template operator()<CInputWin>              ("input",          "vgui2.so",                   "VGUI_Input");
	csystem         = get.template operator()<CSystem>                ("csystem",        "vgui2.so",                   "VGUI_System");

	careerUI        = get.template operator()<CCareerGame>            ("careerUI",       "svencoop/cl_dlls/gameui.so", "CareerUI");
	console         = get.template operator()<CGameConsole>           ("console",        "svencoop/cl_dlls/gameui.so", "GameConsole");
	musicManager    = get.template operator()<MusicManager>           ("musicManager",   "svencoop/cl_dlls/gameui.so", "MusicManager");
	gameEngine      = get.template operator()<CRunGameEngine>         ("gameEngine",     "svencoop/cl_dlls/gameui.so", "RunGameEngine");
	moduleLoader    = get.template operator()<CVGuiSystemModuleLoader>("moduleLoader",   "svencoop/cl_dlls/gameui.so", "VGuiModuleLoader");

	serverBrowser   = get.template operator()<CServerBrowser>         ("serverBrowser",  "platform/servers/serverbrowser_linux.so", "ServerBrowser");
	cvar            = get.template operator()<CDefaultCvar>           ("cvar",           "platform/servers/serverbrowser_linux.so", "VEngineCvar");

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
			fmt::print("{0} - {1:p}\n", cur_interface->m_pName, reinterpret_cast<void*>(&cur_interface->m_CreateFn));
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

template <class T>
static T GetSymbol(std::string filename, std::string symbol)
{
	void* handle = dlopen(filename.c_str(), RTLD_NOW | RTLD_GLOBAL);
	if (handle == nullptr)
		throw Exception("GetSymbol() Failed to get handle of file '{}'", filename);

	void* p = dlsym(handle, symbol.c_str());
	dlclose(handle);
	if (p == nullptr)
		throw Exception("GetSymbol() Could not get symbol address for '{0}' in '{1}'", Demangle(symbol.c_str()), filename);

	fmt::print("({1}) found '{0}'\n", Demangle(symbol.c_str()), filename);
	
	return reinterpret_cast<T>(p);
}

void Interface::FindClientDLLFuncs()
{
	gClientDllFuncs = new ClientDLLFuncs{
	.Init =                     GetSymbol<ClientDLLFuncs::InitFn>("hw.so"s, "_Z14ClientDLL_Initv"s),
	.GetUserEntity =            GetSymbol<ClientDLLFuncs::GetUserEntityFn>("hw.so"s, "_Z23ClientDLL_GetUserEntityi"s),
	.VoiceStatus =              GetSymbol<ClientDLLFuncs::VoiceStatusFn>("hw.so"s, "_Z21ClientDLL_VoiceStatusii"s),
	.ChatInputPosition =        GetSymbol<ClientDLLFuncs::ChatInputPositionFn>("hw.so"s, "_Z27ClientDLL_ChatInputPositionPiS_"s),
	.TempEntUpdate =            GetSymbol<ClientDLLFuncs::TempEntUpdateFn>("hw.so"s, "_Z23ClientDLL_TempEntUpdatedddPP9tempent_sS1_PFiP11cl_entity_sEPFvS0_fE"s),
	.DirectorMessage =          GetSymbol<ClientDLLFuncs::DirectorMessageFn>("hw.so"s, "_Z25ClientDLL_DirectorMessageiPv"s),
	.KeyEvent =                 GetSymbol<ClientDLLFuncs::KeyEventFn>("hw.so"s, "_Z19ClientDLL_Key_EventiiPKc"s),
	.GetHullBounds =            GetSymbol<ClientDLLFuncs::GetHullBoundsFn>("hw.so"s, "_Z23ClientDLL_GetHullBoundsiPfS_"s),
	.ConnectionLessPacket =     GetSymbol<ClientDLLFuncs::ConnectionLessPacketFn>("hw.so"s, "_Z30ClientDLL_ConnectionlessPacketPK8netadr_sPKcPcPi"s),
	.PostRunCmd =               GetSymbol<ClientDLLFuncs::PostRunCmdFn>("hw.so"s, "_Z20ClientDLL_PostRunCmdP13local_state_sS0_P9usercmd_sidj"s),
	.StudioEvent =              GetSymbol<ClientDLLFuncs::StudioEventFn>("hw.so"s, "_Z21ClientDLL_StudioEventPK14mstudioevent_sPK11cl_entity_s"s),
	.DrawNormalTriangles =      GetSymbol<ClientDLLFuncs::DrawNormalTrianglesFn>("hw.so"s, "_Z29ClientDLL_DrawNormalTrianglesv"s),
	.DrawTransparentTriangles = GetSymbol<ClientDLLFuncs::DrawTransparentTrianglesFn>("hw.so"s, "_Z34ClientDLL_DrawTransparentTrianglesv"s),
	.CreateEntities =           GetSymbol<ClientDLLFuncs::CreateEntitiesFn>("hw.so"s, "_Z24ClientDLL_CreateEntitiesv"s),
	.AddEntity =                GetSymbol<ClientDLLFuncs::AddEntityFn>("hw.so"s, "_Z19ClientDLL_AddEntityiP11cl_entity_s"s),
	.CalcRefDef =               GetSymbol<ClientDLLFuncs::CalcRefDefFn>("hw.so"s, "_Z20ClientDLL_CalcRefdefP12ref_params_s"s),
	.CamThink =                 GetSymbol<ClientDLLFuncs::CamThinkFn>("hw.so"s, "_Z19ClientDLL_CAM_Thinkv"s),
	.FindKey =                  GetSymbol<ClientDLLFuncs::FindKeyFn>("hw.so"s, "_Z17ClientDLL_FindKeyPKc"s),
	.GetCameraOffsets =         GetSymbol<ClientDLLFuncs::GetCameraOffsetsFn>("hw.so"s, "_Z26ClientDLL_GetCameraOffsetsPf"s),
	.IsThirdPerson =            GetSymbol<ClientDLLFuncs::IsThirdPersonFn>("hw.so"s, "_Z23ClientDLL_IsThirdPersonv"s),
	.CreateMove =               GetSymbol<ClientDLLFuncs::CreateMoveFn>("hw.so"s, "_Z20ClientDLL_CreateMovefP9usercmd_si"s),
	.ActivateMouse =            GetSymbol<ClientDLLFuncs::ActivateMouseFn>("hw.so"s, "_Z23ClientDLL_ActivateMousev"s),
	.DeactivateMouse =          GetSymbol<ClientDLLFuncs::DeactivateMouseFn>("hw.so"s, "_Z25ClientDLL_DeactivateMousev"s),
	.MouseEvent =               GetSymbol<ClientDLLFuncs::MouseEventFn>("hw.so"s, "_Z20ClientDLL_MouseEventi"s),
	.ClearStates =              GetSymbol<ClientDLLFuncs::ClearStatesFn>("hw.so"s, "_Z21ClientDLL_ClearStatesv"s),
	.Accumulate =               GetSymbol<ClientDLLFuncs::AccumulateFn>("hw.so"s, "_Z23ClientDLL_IN_Accumulatev"s),
	.ClientMoveInit =           GetSymbol<ClientDLLFuncs::ClientMoveInitFn>("hw.so"s, "_Z24ClientDLL_ClientMoveInitP12playermove_s"s),
	.ClientTextureType =        GetSymbol<ClientDLLFuncs::ClientTextureTypeFn>("hw.so"s, "_Z27ClientDLL_ClientTextureTypePc"s),
	.ClientMove =               GetSymbol<ClientDLLFuncs::ClientMoveFn>("hw.so"s, "_Z20ClientDLL_MoveClientP12playermove_s"s),
	.Shutdown =                 GetSymbol<ClientDLLFuncs::ShutdownFn>("hw.so"s, "_Z18ClientDLL_Shutdownv"s),
	.HudVidInit =               GetSymbol<ClientDLLFuncs::HudVidInitFn>("hw.so"s, "_Z20ClientDLL_HudVidInitv"s),
	//_Z17ClientDLL_HudInitv
	.HudRedraw =                GetSymbol<ClientDLLFuncs::HudRedrawFn>("hw.so"s, "_Z19ClientDLL_HudRedrawi"s),
	.Frame =                    GetSymbol<ClientDLLFuncs::FrameFn>("hw.so"s, "_Z15ClientDLL_Framed"s),
	.UpdateClientData =         GetSymbol<ClientDLLFuncs::UpdateClientDataFn>("hw.so"s, "_Z26ClientDLL_UpdateClientDatav"s),
	//_Z30ClientDLL_DemoUpdateClientDataP13client_data_s
	.TxferLocalOverrides =      GetSymbol<ClientDLLFuncs::TxferLocalOverridesFn>("hw.so"s, "_Z29ClientDLL_TxferLocalOverridesP14entity_state_sPK12clientdata_s"s),
	.ProcessPlayerState =       GetSymbol<ClientDLLFuncs::ProcessPlayerStateFn>("hw.so"s, "_Z28ClientDLL_ProcessPlayerStateP14entity_state_sPKS_"s),
	.TxferPredictionData =      GetSymbol<ClientDLLFuncs::TxferPredictionDataFn>("hw.so"s, "_Z29ClientDLL_TxferPredictionDataP14entity_state_sPKS_P12clientdata_sPKS3_P13weapon_data_sPKS7_"s),
	.ReadDemoBuffer =           GetSymbol<ClientDLLFuncs::ReadDemoBufferFn>("hw.so"s, "_Z24ClientDLL_ReadDemoBufferiPh"s),
	//_Z30ClientDLL_CheckStudioInterfacePv
	};
}

GetLocalPlayerFn GetLocalPlayer = nullptr;

void Interface::FindFunctions()
{
	//GetLocalPlayer = GetSymbol<GetLocalPlayerFn>("hw.so"s, "_Jv_RegisterClasses"s);
}
