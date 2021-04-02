#include "Util.hpp"
#include <link.h>
#include <unistd.h>
#include "VMT.hpp"

std::string Demangle(const char* name)
{
	std::unique_ptr<char, void (*)(void*)> own // Get readable type name
		(abi::__cxa_demangle(name, nullptr, nullptr, nullptr), std::free);
	return own != nullptr ? own.get() : name;
}

Elf64_Word GetProtectionFlags(uintptr_t address)
{
	static Elf64_Word flags = 0;
	static std::uintptr_t addr = address;

	dl_iterate_phdr([](struct dl_phdr_info* info, std::size_t, void*)
	{
		std::uintptr_t startingAddr = 0;
		std::uintptr_t endingAddr = 0;

		for (Elf64_Half i = 0; i < info->dlpi_phnum; i++)
		{
			const ElfW(Phdr) *hdr = &info->dlpi_phdr[i];
			if (hdr->p_memsz)
			{
				startingAddr = info->dlpi_addr + hdr->p_vaddr;
				endingAddr = startingAddr + hdr->p_memsz;
				if (startingAddr <= addr && endingAddr >= addr)
					flags |= hdr->p_flags;
			}
		}

		return 0;
	}, nullptr);

	return flags;
}

long pageSize = sysconf(_SC_PAGESIZE);
std::pair<std::uintptr_t, std::size_t> getMinimumPage(std::uintptr_t addr, std::size_t len)
{
	std::uintptr_t ret = (addr / pageSize) * pageSize;

	std::size_t n = (addr - ret + len);
	if (n % pageSize)
		n /= pageSize, ++n;
	else
		n /= pageSize;
	
	return {ret, n*pageSize};
}
