#ifndef ENGINE_PRIMITIVES_CLIENTSPRITE_HPP
#define ENGINE_PRIMITIVES_CLIENTSPRITE_HPP

#include "Defs.hpp"
#include "WRect.hpp"

struct ClientSprite
{
	static constexpr inline std::size_t NameLen = 64;
	char name[NameLen];
	static constexpr inline std::size_t SpriteLen = 64;
	char sprite[SpriteLen];
	HSprite hspr;
	i32 iRes;
	WRect rect;
};

#endif // ENGINE_PRIMITIVES_CLIENTSPRITE_HPP
