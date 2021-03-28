#ifndef UTIL_HPP
#define UTIL_HPP

#include <cstdint>
#include <string>
#include <cstring>
#include <elf.h>
#include <dlfcn.h>
#include <cxxabi.h>
#include <codecvt>
#include <locale>
#include <fmt/format.h>
#include "Cenum.hpp"
#include "Math.hpp"


template <typename T>
std::string type_name()
{
	typedef typename std::remove_reference<T>::type TR;
	std::unique_ptr<char, void (*)(void*)> own // Get readable type name
		(abi::__cxa_demangle(typeid(TR).name(), nullptr, nullptr, nullptr), std::free);
	return own != nullptr ? own.get() : typeid(TR).name();
}

class Exception
{
	std::string m_msg;

public:
	Exception(const std::string& format, auto... args)
	{
		m_msg = fmt::format(format, args...);
	}

	virtual std::string what() const throw()
	{
		return m_msg;
	}
};

inline void**& getvtable(void* inst, std::size_t offset = 0)
{
	return *reinterpret_cast<void***>((std::size_t)inst + offset);
}

template<class Fn>
inline Fn getvfunc(void* inst, std::size_t index, std::size_t offset = 0)
{
    return reinterpret_cast<Fn>(getvtable(inst, offset)[index]);
}

typedef void* (*InstantiateInterfaceFn) ();

struct InterfaceReg
{
	InstantiateInterfaceFn m_CreateFn;
	const char *m_pName;
	InterfaceReg *m_pNext;
};

template <class I>
I* GetInterface(const char* filename, const char* version, bool exact = false)
{
	void* library = dlopen(filename, RTLD_NOLOAD | RTLD_NOW | RTLD_LOCAL);

	if (!library)
		return nullptr;

	void* interfaces_sym = dlsym(library, "_ZN12InterfaceReg16s_pInterfaceRegsE"); // gcc mangling

	dlclose(library);
	if (interfaces_sym == nullptr)
		return nullptr;

	InterfaceReg* interfaces = *reinterpret_cast<InterfaceReg**>(interfaces_sym);

	InterfaceReg* cur_interface;

	for (cur_interface = interfaces; cur_interface != nullptr; cur_interface = cur_interface->m_pNext)
	{
		if (exact)
		{
			if (strcmp(cur_interface->m_pName, version) == 0)
				return reinterpret_cast<I*>(cur_interface->m_CreateFn());
		}
		else
		{
			if (strstr(cur_interface->m_pName, version) && strlen(cur_interface->m_pName) - 3 == strlen(version))
				return reinterpret_cast<I*>(cur_interface->m_CreateFn());
		}
	}

	return nullptr;
}

inline std::uintptr_t GetAbsoluteAddress(std::uintptr_t instruction_ptr, std::size_t offset, std::size_t size)
{
	return instruction_ptr + *reinterpret_cast<i32*>(instruction_ptr + offset) + size;
};

template <class T>
T GetSymbolAddress(const char* filename, const char* symbol)
{
	void* handle = dlopen(filename, RTLD_NOW | RTLD_LOCAL);
	if (handle == nullptr)
		throw Exception("GetSymbolAddress() Failed to get handle of file '{}'", filename);

	void* p = dlsym(handle, symbol);
	dlclose(handle);
	if (p == nullptr)
		throw Exception("GetSymbolAddress() Failed to get symbol '{}' in file '{}'", symbol, filename);

	return reinterpret_cast<T>(p);
};

// GNU chads have 4byte wchar_t
template <typename T, typename U>
std::basic_string<T> StringConvert(const std::basic_string<U>& s)
{
	if constexpr (std::is_same_v<T, U>)
		return s;

	std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> conv;
	if constexpr (std::is_same_v<T, wchar_t>)
	{
		if constexpr (std::is_same_v<U, char>)
		{
			try
			{
				return conv.from_bytes(s);
			}
			catch (std::range_error&)
			{
				throw Exception("StringConvert<{0}, {1}>() Failed, could not convert '{2}'", type_name<T>, type_name<U>, s);
			}
		}
	}
	else if constexpr (std::is_same_v<T, char>)
	{
		if constexpr (std::is_same_v<U, wchar_t>)
		{
			try
			{
				return conv.to_bytes(s);
			}
			catch (std::range_error&)
			{
				throw Exception("StringConvert<{0}, {1}>() Failed, could not convert '{2}'", type_name<T>, type_name<U>, s);
			}
		}
	}
}

Elf64_Word GetProtectionFlags(std::uintptr_t address);

extern long pageSize;
std::pair<std::uintptr_t, std::size_t> getMinimumPage(std::uintptr_t addr, std::size_t len);

#endif // UTIL_HPP
