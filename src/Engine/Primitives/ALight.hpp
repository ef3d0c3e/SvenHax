#ifndef ENGINE_PRIMITIVES_ALIGHT_HPP
#define ENGINE_PRIMITIVES_ALIGHT_HPP

#include "Defs.hpp"
#include "Color.hpp"

struct ALight
{
	i32 ambientLight; // clip at 128
	i32 shadeLight; // clip at 192 - ambientlight
	Color3f color;
	f32* pLightVec;
};

#endif // ENGINE_PRIMITIVES_ALIGHT_HPP
