#ifndef ENGINE_PRIMITIVES_CLIENTTEXTMESSAGE_HPP
#define ENGINE_PRIMITIVES_CLIENTTEXTMESSAGE_HPP

#include "Defs.hpp"
#include "Color.hpp"
#include "Vector.hpp"

struct ClientTextMessage
{

	i32 effect;
	Color32 col1; // 2 colros for effects
	Color32 col2;
	Vec2 pos;
	f32 fadeIn;
	f32 fadeOut;
	f32 holdTime;
	f32 fxTime;
	const char *name;
	const char *message;
};

#endif // ENGINE_PRIMITIVES_CLIENTTEXTMESSAGE_HPP
