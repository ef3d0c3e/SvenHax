#ifndef HOOKER_HPP
#define HOOKER_HPP

#include <sys/mman.h>
#include <cstring>
#include <vector>
#include "Util.hpp"

class Tramp
{
	std::uintptr_t m_gateway = 0;
	std::uintptr_t m_addr = 0;
	std::size_t m_len;
public:
	Tramp(std::uintptr_t fn, std::uintptr_t hkFn, std::size_t len, std::function<void(std::uintptr_t, std::uintptr_t)> patch)
	{
		m_addr = fn;
		m_len = len;

		if (m_len < 5)
			throw Exception("Tramp::Tramp() len is smaller than 5\n");

		// Allocate array that can hold the original opcodes
		const std::size_t gatewayLen = m_len + 5;
		m_gateway = reinterpret_cast<std::uintptr_t>(new u8[gatewayLen]);

		const auto prot = GetProtectionFlags(m_addr);
		// Set protection
		if (auto&& [addr, len] = getMinimumPage(m_addr, m_len); mprotect(reinterpret_cast<void*>(addr), len, PROT_READ | PROT_WRITE | PROT_EXEC) == -1)
			throw Exception("Tramp::Tramp() mprotect(addr) failed to set writable\n");

		// Copy bytes & Write new bytes
		std::memcpy(reinterpret_cast<void*>(m_gateway), reinterpret_cast<void*>(m_addr), m_len);
		reinterpret_cast<u8*>(m_gateway)[m_len] = 0xE9; // jump near
		*reinterpret_cast<u32*>(m_gateway+m_len+1) =  m_addr - m_gateway - 5;
		
		// Patches
		patch(m_addr, m_gateway);

		std::memset(reinterpret_cast<void*>(m_addr), 0x90, m_len);
		reinterpret_cast<u8*>(m_addr)[0] = 0xE9; // jump near
		*reinterpret_cast<u32*>(m_addr+1) = hkFn - m_addr - 5;

		// Revert original protection
		if (auto&& [addr, len] = getMinimumPage(m_addr, m_len); mprotect(reinterpret_cast<void*>(addr), len, prot) == -1)
			throw Exception("Tramp::Tramp() mprotect(addr) failed to restore permissions\n");

		// Make the gateway executable
		if (auto&& [addr, len] = getMinimumPage(reinterpret_cast<std::uintptr_t>(m_gateway), gatewayLen); mprotect(reinterpret_cast<void*>(addr), len, PROT_READ | PROT_WRITE | PROT_EXEC) == -1)
			throw Exception("Tramp::Tramp() mprotect(gateway) failed to set executable\n");
	}

	void Release(std::function<void(std::uintptr_t, std::uintptr_t)> patch)
	{

		const auto prot = GetProtectionFlags(m_addr);
		// Set protection
		if (auto&& [addr, len] = getMinimumPage(m_addr, m_len); mprotect(reinterpret_cast<void*>(addr), len, PROT_READ | PROT_WRITE | PROT_EXEC) == -1)
			throw Exception("Tramp::~Tramp() mprotect(addr) failed to set writable\n");

		// Write the original bytes
		std::memcpy(reinterpret_cast<void*>(m_addr), reinterpret_cast<void*>(m_gateway), m_len);

		patch(m_addr, m_gateway);

		// Revert original protection
		if (auto&& [addr, len] = getMinimumPage(m_addr, m_len); mprotect(reinterpret_cast<void*>(addr), len, prot) == -1)
			throw Exception("Tramp::~Tramp() mprotect(addr) failed to restore permissions\n");

		// Free memory
		delete[] reinterpret_cast<u8*>(m_gateway);
	}

	~Tramp()
	{
		Release([](std::uintptr_t, std::uintptr_t){});
	}

	template <class Fn>
	inline Fn GetOriginalFunction()
	{
		return reinterpret_cast<Fn>(m_gateway);
	}
};

#endif // HOOKER_HPP
