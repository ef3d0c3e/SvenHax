#ifndef ENGINE_PRIMITIVES_EFRAG_HPP
#define ENGINE_PRIMITIVES_EFRAG_HPP

#include "Vector.hpp"
struct MLeaf;

struct EFrag
{
	MLeaf* leaf;
	EFrag* leafNext;
	Entity* entity;
	EFrag* entNext;
};

#endif // ENGINE_PRIMITIVES_EFRAG_HPP
