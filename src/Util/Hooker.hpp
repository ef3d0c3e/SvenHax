#ifndef HOOKER_HPP
#define HOOKER_HPP

#include <sys/mman.h>
#include <cstring>
#include <vector>
#include "Util.hpp"

template <bool Far>
class Tramp
{
	std::uintptr_t m_gateway = 0;
	std::uintptr_t m_addr = 0;
	std::size_t m_len;
public:
	Tramp(std::uintptr_t fn, std::uintptr_t hkFn, std::size_t len)
	{
		m_addr = fn;
		m_len = len;

		if constexpr (Far)
		{
			if (m_len < 12)
				throw Exception("Tramp::Tramp() len is smaller than 12\n");

			u8 opcodes[12] = {
				0x48, 0xB8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // movabs $addr,%rax
				0xFF, 0xE0, // jmp *%rax
			};
			auto setOpcodes = [&opcodes](std::uintptr_t addr)
			{
				*reinterpret_cast<u64*>(opcodes+2) = static_cast<u64>(addr);
			};

			// Allocate array that can hold the original opcodes
			const std::size_t gatewayLen = m_len + 12;
			m_gateway = reinterpret_cast<std::uintptr_t>(new u8[gatewayLen]);

			const auto prot = GetProtectionFlags(m_addr);
			// Set protection
			if (auto&& [addr, len] = getMinimumPage(m_addr, m_len); mprotect(reinterpret_cast<void*>(addr), m_len, PROT_READ | PROT_WRITE | PROT_EXEC) == -1)
				throw Exception("Tramp::Tramp() mprotect(addr) failed to set writable\n");

			std::memcpy(reinterpret_cast<void*>(m_gateway), reinterpret_cast<void*>(m_addr), m_len); // Copy original bytes
			setOpcodes(m_addr+m_len);
			std::memcpy(reinterpret_cast<void*>(m_gateway+m_len), opcodes, 12); // Writes jump opcodes

			std::memset(reinterpret_cast<void*>(m_addr), 0x90, m_len); // Replace stolen with NOPs
			setOpcodes(hkFn);
			std::memcpy(reinterpret_cast<void*>(m_addr), opcodes, 12); // Writes jump opcodes

			// Revert original protection
			if (auto&& [addr, len] = getMinimumPage(m_addr, m_len); mprotect(reinterpret_cast<void*>(addr), m_len, prot) == -1)
				throw Exception("Tramp::Tramp() mprotect(addr) failed to restore permissions\n");

			// Make the gateway executable
			if (auto&& [addr, len] = getMinimumPage(reinterpret_cast<std::uintptr_t>(m_gateway), gatewayLen); mprotect(reinterpret_cast<void*>(addr), m_len, PROT_READ | PROT_WRITE | PROT_EXEC) == -1)
				throw Exception("Tramp::Tramp() mprotect(gateway) failed to set executable\n");
		}
		else
		{
			if (m_len < 5)
				throw Exception("Tramp::Tramp() len is smaller than 5\n");

			// Allocate array that can hold the original opcodes
			const std::size_t gatewayLen = m_len + 5;
			m_gateway = reinterpret_cast<std::uintptr_t>(new u8[gatewayLen]);

			const auto prot = GetProtectionFlags(m_addr);
			// Set protection
			if (auto&& [addr, len] = getMinimumPage(m_addr, m_len); mprotect(reinterpret_cast<void*>(addr), m_len, PROT_READ | PROT_WRITE | PROT_EXEC) == -1)
				throw Exception("Tramp::Tramp() mprotect(addr) failed to set writable\n");

			// Copy bytes & Write new bytes
			std::memcpy(reinterpret_cast<void*>(m_gateway), reinterpret_cast<void*>(m_addr), m_len);
			reinterpret_cast<u8*>(m_gateway)[m_len] = 0xE9; // jump near
			*reinterpret_cast<u32*>(m_gateway+m_len+1) = m_addr - reinterpret_cast<std::uintptr_t>(m_gateway) - m_len;

			std::memset(reinterpret_cast<void*>(m_addr), 0x90, m_len);
			reinterpret_cast<u8*>(m_addr)[0] = 0xE9; // jump near
			*reinterpret_cast<u64*>(m_addr+1) = hkFn - m_addr - 5;

			// Revert original protection
			if (auto&& [addr, len] = getMinimumPage(m_addr, m_len); mprotect(reinterpret_cast<void*>(addr), len, prot) == -1)
				throw Exception("Tramp::Tramp() mprotect(addr) failed to restore permissions\n");

			// Make the gateway executable
			if (auto&& [addr, len] = getMinimumPage(reinterpret_cast<std::uintptr_t>(m_gateway), gatewayLen); mprotect(reinterpret_cast<void*>(addr), len, PROT_READ | PROT_WRITE | PROT_EXEC) == -1)
				throw Exception("Tramp::Tramp() mprotect(gateway) failed to set executable\n");
		}
	}

	void Release()
	{

		const auto prot = GetProtectionFlags(m_addr);
		// Set protection
		if (auto&& [addr, len] = getMinimumPage(m_addr, m_len); mprotect(reinterpret_cast<void*>(addr), len, PROT_READ | PROT_WRITE | PROT_EXEC) == -1)
			throw Exception("Tramp::~Tramp() mprotect(addr) failed to set writable\n");

		// Write the original bytes
		std::memcpy(reinterpret_cast<void*>(m_addr), reinterpret_cast<void*>(m_gateway), m_len);

		// Revert original protection
		if (auto&& [addr, len] = getMinimumPage(m_addr, m_len); mprotect(reinterpret_cast<void*>(addr), len, prot) == -1)
			throw Exception("Tramp::~Tramp() mprotect(addr) failed to restore permissions\n");

		// Free memory
		delete[] reinterpret_cast<u8*>(m_gateway);
	}

	~Tramp()
	{
		Release();
	}

	template <class Fn>
	Fn  __attribute__((always_inline)) GetOriginalFunction()
	{
		return reinterpret_cast<Fn>(m_gateway);
	}
};

#endif // HOOKER_HPP
