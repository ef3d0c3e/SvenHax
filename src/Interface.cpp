#include "Interface.hpp"

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
#include <fmt/format.h>

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
		void* library = dlopen(module, RTLD_NOLOAD | RTLD_NOW);

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

std::uintptr_t Interface::BaseAddr = 0;
std::uintptr_t Interface::GetBaseAddress()
{

	// Create temp buffers
	char* memoryBuffer = new char[32];
	char* memoryAddressBuffer = new char[32];

	// Create file path
	char filePath[64];
	sprintf(filePath, "/proc/%d/maps", getpid());

	// Open target virtual memory
	int fd = open(filePath, O_RDONLY);
	if (!fd)
		return 0;

	// Read memory into buffer
	if (read(fd, memoryBuffer, sizeof(char) * 32) == -1)
	{
		std::string msg;
		switch (errno)
		{
			case EAGAIN:
				msg = "The file descriptor fd refers to a file other than a socket and has been marked nonblocking (O_NONBLOCK), and the read would block.  See open(2) for further details on the O_NONBLOCK flag. The file descriptor fd refers to a socket and has been marked nonblocking (O_NONBLOCK), and the read would block.  POSIX.1-2001 allows either error to be returned for this case,  and  does  not require these constants to have the same value, so a portable application should check for both possibilities.";
				break;
			case EBADF:
				msg = "fd is not a valid file descriptor or is not open for reading.";
				break;
			case EFAULT: 
				msg = "buf is outside your accessible address space.";
				break;
			case EINTR:
				msg = "The call was interrupted by a signal before any data was read; see signal(7).";
				break;
			case EINVAL:
				msg = "fd  is  attached  to an object which is unsuitable for reading; or the file was opened with the O_DIRECT flag, and either the address specified in buf, the value specified in count, or the file offset is not suitably aligned. fd was created via a call to timerfd_create(2) and the wrong size buffer was given to read(); see timerfd_create(2) for further information.";
				break;
			case EIO:
				msg = "I/O error.  This will happen for example when the process is in a background process group, tries to read from its controlling terminal, and either it is ignoring or  blocking  SIGTTIN  or  its process  group  is orphaned.  It may also occur when there is a low-level I/O error while reading from a disk or tape.  A further possible cause of EIO on networked filesystems is when an advisory lock had been taken out on the file descriptor and this lock has been lost.  See the Lost locks section of fcntl(2) for further details.";
				break;
			case EISDIR:
				msg = "fd refers to a directory.";
				break;
		}
		throw Exception(msg);
	}

	int StatusIndex = 0;

	// Scan map
	while (true)
	{
		// Check for null byte or newline character
		if (memoryBuffer[StatusIndex] == '-' || memoryBuffer[StatusIndex] == '\n' || memoryBuffer[StatusIndex] == 0)
		{
			memoryAddressBuffer[StatusIndex] = 0;
			break;
		}

		// Read memory to szProcessNameBuffer
		memoryAddressBuffer[StatusIndex] = memoryBuffer[StatusIndex];
		StatusIndex++;
	}

	// Close fd
	close(fd);

	// Convert ascii to hex address
	std::uintptr_t baseAddress = static_cast<std::uintptr_t>(strtol(memoryAddressBuffer, NULL, 16));

	// Free memory
	delete[] memoryBuffer;
	delete[] memoryAddressBuffer;

	return baseAddress;
}
