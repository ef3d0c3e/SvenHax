#ifndef CONFIG_HPP
#define CONFIG_HPP

#include "Engine/Primitives/Defs.hpp"
#include "ImGui/imgui.h"
#include "Util/Cenum.hpp"
#include <map>
#include <string>

namespace Config
{
template <std::size_t N>
struct VarID
{
	constexpr VarID(const char (&literal)[N])
	{
		std::copy_n(literal, N, value);
	}

	constexpr explicit operator std::string() const
	{
		return std::string(value, N);
	}

	char value[N];
};

struct Var
{
	MAKE_CENUMV_Q(Type, u8,
		BOOL, 0,
		I32, 1,
		F32, 2,
		IMVEC4, 3,
		STRING, 4,
	) type;
	void* p;

	template <typename T>
	constexpr Var(T* ptr)
	: type(GetType<T>()), p(ptr)
	{}

	constexpr Var()
	: type(0), p(nullptr)
	{}

	template <class T>
	consteval static Type GetType()
	{

		if constexpr (std::is_same_v<T, bool>)
		{
			return Type::BOOL;
		}
		else if constexpr (std::is_convertible_v<T, i32>)
		{
			return Type::I32;
		}
		else if constexpr (std::is_same_v<T, f32>)
		{
			return Type::F32;
		}
		else if constexpr (std::is_same_v<T, ImVec4>)
		{
			return Type::IMVEC4;
		}
		else
		{
			[]<bool v = false>()
			{
				static_assert(v, "Unsupported type");
			}
			();
		}
	}
};

extern std::map<std::string, Var> varList;

void Load(const std::string& name);
void Save(const std::string& name);
}

template <Config::VarID id, class T>
void Export(T& t)
{
	static bool init = false;
	if (init)
		return;

	Config::varList[static_cast<std::string>(id)] = Config::Var(&t);
}

#endif // CONFIG_HPP
