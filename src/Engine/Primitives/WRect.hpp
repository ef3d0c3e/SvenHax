#ifndef ENGINE_PRIMITIVES_WRECT_HPP
#define ENGINE_PRIMITIVES_WRECT_HPP

#include "Vector.hpp"

struct WRect
{
	Vec2i min, max; // (left; right), (top bottom)
};

#endif // ENGINE_PRIMITIVES_WRECT_HPP
