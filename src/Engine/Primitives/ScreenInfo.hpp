#ifndef ENGINE_PRIMITIVES_SCREENINFO_HPP
#define ENGINE_PRIMITIVES_SCREENINFO_HPP

#include "Defs.hpp"

struct ScreenInfo
{
	i32 size;
	i32 width;
	i32 height;
	i32 flags;
	i32 charHeight;
	static constexpr inline std::size_t CharWidthNum = 256;
	i16 charWidths[CharWidthNum];

};

#endif // ENGINE_PRIMITIVES_SCREENINFO_HPP
