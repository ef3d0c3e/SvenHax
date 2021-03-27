#include "Patterns.hpp"
#include "Util.hpp"
#include <link.h>
#include <dlfcn.h>
#include <vector>
#include <string.h>
#include <sstream>

using namespace std::literals;

struct dlinfo_t
{
	const char* library = nullptr;
	std::uintptr_t address = 0;
	std::size_t size = 0;
};

std::vector<dlinfo_t> libraries;

std::tuple<bool, std::uintptr_t, std::size_t> Hooker::GetLibraryInformation(const char* library)
{
	if (libraries.size() == 0)
	{
		dl_iterate_phdr([] (struct dl_phdr_info* info, size_t, void*)
		{
			dlinfo_t library_info = {};

			library_info.library = info->dlpi_name;
			library_info.address = info->dlpi_addr + info->dlpi_phdr[0].p_vaddr;
			library_info.size = info->dlpi_phdr[0].p_memsz;

			libraries.push_back(library_info);

			return 0;
		}, nullptr);
	}

	for (const dlinfo_t& current: libraries)
	{
		if (!strcasestr(current.library, library))
			continue;

		return {true, current.address, current.size};
	}

	return {false, std::uintptr_t(), std::size_t()};
}

std::uintptr_t Hooker::FindPattern(const std::function<bool(const std::string&)> pathFn, Maps::Permissions permMask, std::uintptr_t beg, std::uintptr_t end, const std::u8string_view& pattern, const std::u8string_view& mask)
{
	if (pattern.size() != mask.size())
		throw Exception("Hooker::FindPattern(): Mask and pattern do not have the same size");
	if (pattern.size() > beg-end)
		throw Exception("Hooker::FindPattern(): Mask is bigger than the range");
	
	auto Compare = [&](std::uintptr_t addr) -> std::size_t
	{
		if (pattern.size() == 0)
			return 0;

		for (std::size_t i = 0; i < pattern.size(); ++i)
		{
			if (mask[i] == u8'x' && *(reinterpret_cast<char8_t*>(addr+i)) != pattern[i])
				return i;
		}

		return pattern.size();
	};

	for (const auto& ent : Maps::mapsEntries)
	{
		if ((ent.perm & permMask) != permMask ||
			ent.size < pattern.size() ||
			ent.address+ent.size-pattern.size() <= beg ||
			!pathFn(ent.path))
			continue;

		std::cout << std::hex << ent.address << "/" << std::hex << ent.size << " -- " <<  ent.path << "\n";

		std::uintptr_t i = 0;
		while (i < ent.size-pattern.size())
		{
			if (i >= end-pattern.size())
				return 0;

			const auto matched = Compare(ent.address + i);
			if (matched == pattern.size())
				return ent.address + i;
			i += matched+1;
		}
	}

	return 0;
}

