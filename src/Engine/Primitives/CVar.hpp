#ifndef ENGINE_PRIMITIVES_CVAR_HPP
#define ENGINE_PRIMITIVES_CVAR_HPP

#include "Defs.hpp"

struct CVar
{
	char* name;
	char* string;
	i32 flags;
	f32 value;
	CVar* next;
};

#endif // ENGINE_PRIMITIVES_CVAR_HPP
