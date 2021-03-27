#ifndef VMT_HPP
#define VMT_HPP

#include <link.h>
#include <elf.h>
#include <vector>
#include "Util.hpp"

inline std::size_t CountVMs(void* interface)
{
	std::uintptr_t** vmt = reinterpret_cast<std::uintptr_t**>(interface);

	std::size_t methodCount = 0;

	while (vmt && (*vmt)[methodCount] && ( GetProtectionFlags( (*vmt)[methodCount] ) & PF_X ))
		methodCount++;

	return methodCount;
}

// Based on https://web.archive.org/web/20170701021444/https://spockwangs.github.io/2011/01/31/cpp-object-model.html
class VMT
{
	std::uintptr_t* m_vmt;
	std::uintptr_t** m_interface = nullptr;
	std::uintptr_t* m_originalVmt = nullptr;
	std::size_t m_methodCount = 0;
	bool m_hasRTTI = false;
	i32 m_rttiPrefix = 0;

	static inline std::vector<VMT*> m_vmts;
public:

	~VMT()
	{
		ReleaseVMT();
		delete[] m_vmt;
	}

	// rttiPrefix 2??? Wtf? Yeah read this: https://web.archive.org/web/20170701021444/https://spockwangs.github.io/2011/01/31/cpp-object-model.html
	explicit VMT(void* interface, bool copyRTTI = true, i32 rttiPrefixAmount = 2)
	{
		m_interface = reinterpret_cast<std::uintptr_t**>(interface);

		m_methodCount = CountVMs(interface) + 2 + rttiPrefixAmount;

		m_originalVmt = *m_interface;

		// Copy the Original Vtable.
		if (copyRTTI)
		{
			m_vmt = new std::uintptr_t[m_methodCount + 1];
			std::memcpy(m_vmt, &m_originalVmt[-rttiPrefixAmount], (sizeof(std::uintptr_t) * m_methodCount) + sizeof(std::uintptr_t));
			m_hasRTTI = true;
			m_rttiPrefix = rttiPrefixAmount;
		}
		else
		{
			m_vmt = new std::uintptr_t[m_methodCount];
			std::memcpy(m_vmt, m_originalVmt, sizeof(std::uintptr_t) * m_methodCount);
		}

		// Make sure to "NULL terminate" our new array of pointers.
		std::memset(&m_vmt[m_methodCount], 0x0, sizeof(std::uintptr_t));

		m_vmts.push_back(this);
	}

	// Hook virtual method
	template <class Fn>
	void HookVM(Fn method, std::size_t methodIndex)
	{
		m_vmt[m_hasRTTI ? methodIndex + m_rttiPrefix : methodIndex] = reinterpret_cast<std::uintptr_t>(method);
	}

	template<class Fn>
	Fn GetOriginalMethod(std::size_t methodIndex)
	{
		return reinterpret_cast<Fn>(m_originalVmt[methodIndex]);
	}

	void ApplyVMT()
	{
		if (m_hasRTTI)
			*m_interface = &m_vmt[m_rttiPrefix];
		else
			*m_interface = m_vmt;
	}

	void ReleaseVMT()
	{
		if (!m_interface)
			return;
		if (*m_interface && m_originalVmt)
			*m_interface = m_originalVmt;
	}

	std::size_t MethodCount() const
	{
		return m_methodCount;
	}

	static void ReleaseAllVMTs()
	{
		for (auto& vmt : m_vmts)
			delete vmt;
		m_vmts.clear();
	}
};

#endif // VMT_HPP
