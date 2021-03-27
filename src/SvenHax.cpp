#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <fstream>
#include <unistd.h>
#include <fmt/format.h>

#include "Util/Maps.hpp"
#include "Util/VMT.hpp"
#include "Interface.hpp"
#include "Hooks/Hooks.hpp"

std::ofstream Log;
std::mutex mtx;
std::condition_variable cv;

template <class String>
inline void Message(const String& format, const auto&... args)
{
	std::cout << "\033[32m> ";
	fmt::print(format, args...);
	std::cout << "\033[0m\n";
}

VMT* engineVGuiVMT = nullptr;

void MainThread()
{
	Message("Loaded");
	//TODO: wait till we can use ofstream...
	{
		std::unique_lock<std::mutex> lck(mtx);
		cv.wait(lck, []()
		{
			Log.open("/tmp/svenhax.log");
			return Log.is_open();
		});
	}

	Log << " * PID=" << getpid() << "\n";
	Log.flush();

	try
	{
		Interface::BaseAddr = Interface::GetBaseAddress();
		Log << std::hex << " * Interface::BaseAddr=" << Interface::BaseAddr << "\n";
		Log.flush();
		Message("Base address found: {0:#x}", Interface::BaseAddr);

		Maps::ParseMaps();
		std::cout << "* /proc/self/maps parsed\n";
		Message("Maps parsed");
	
		Interface::DumpInterfaces();
		Interface::FindInterfaces();

		console->DPrintf("SvenHax Successfully Loaded...\n");
	}
	catch (Exception& e)
	{
		std::cerr << "\033[31m***EXCEPTION CAUGHT***\n";
		std::cerr << e.what();
		std::cerr << "\n***END OF EXCEPTION***\033[0m\n";
	}

	Log.close();
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

	Message("Unloaded");
}
