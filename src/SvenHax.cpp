#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <fstream>
#include <unistd.h>
#include <fmt/format.h>

#include "Engine/ClientDLL.hpp"

#include "Util/Maps.hpp"
#include "Util/VMT.hpp"
#include "Interface.hpp"
#include "Hooks/Hooks.hpp"

std::mutex mtx;
std::condition_variable cv;

template <class String>
inline void Message(const String& format, const auto&... args)
{
	std::cout << "\033[32m> ";
	fmt::print(format, args...);
	std::cout << "\033[0m\n";
}

static bool failed = false;;

void MainThread()
{
	Message("Loading...");

	try
	{
		Interface::FindSymbols();

		Interface::DumpInterfaces();
		Interface::FindInterfaces();

		Interface::FindClientDLLFuncs();
		Interface::FindFunctions();

		SDL2::HookSwapWindow();
		SDL2::HookPollEvent();
		CreateMove::HookCreateMove();
		
		console->DPrintf("SvenHax Successfully Loaded...\n");
	}
	catch (Exception& e)
	{
		std::cerr << "\033[31m***EXCEPTION CAUGHT***\n";
		std::cerr << e.what();
		std::cerr << "\n***END OF EXCEPTION***\033[0m\n";
		failed = true;
	}
}

std::thread* mainThread = nullptr;
int __attribute__((constructor)) Startup()
{
	mainThread = new std::thread(MainThread);

	return 0;
}

void __attribute__((destructor)) Shutdown()
{
	mainThread->join();
	VMT::ReleaseAllVMTs();

	if (!failed)
	{
		SDL2::UnhookWindow();
		SDL2::UnhookPollEvent();
		CreateMove::UnhookCreateMove();
	}

	Message("Unloaded");
}
