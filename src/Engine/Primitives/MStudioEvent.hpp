#ifndef ENGINE_PRIMITIVES_MSTUDIOEVENT_HPP
#define ENGINE_PRIMITIVES_MSTUDIOEVENT_HPP

#include "Defs.hpp"

struct MStudioEvent
{
	i32 frame;
	i32 event;
	i32 type;
	static constexpr inline std::size_t OptionsLen = 64;
	char options[OptionsLen];
};

#endif // ENGINE_PRIMITIVES_MSTUDIOEVENT_HPP
