#ifndef ENGINE_PRIMITIVES_MSTUDIOBODYPARTS_HPP
#define ENGINE_PRIMITIVES_MSTUDIOBODYPARTS_HPP

#include "Defs.hpp"
#include <array>

struct MStudioBodyParts
{
	static constexpr inline std::size_t NameLen = 64;
	std::array<char, NameLen> name;
	i32 numModels;
	i32 base;
	i32 modelIndex; // index into models array
};

#endif // ENGINE_PRIMITIVES_MSTUDIOBODYPARTS_HPP
