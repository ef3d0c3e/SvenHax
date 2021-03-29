#ifndef ENGINE_PRIMITIVES_DECAL_HPP
#define ENGINE_PRIMITIVES_DECAL_HPP

#include "Vector.hpp"
struct MSurface;

struct Decal
{
	Decal* pNext; // linked list for each surface
	struct MSurface* pSurface; // Surface id for persistence / unlinking
	i16 dx; // Offsets into surface texture (in texture coordinates, so we don't need floats)
	i16 dy;
	i16 texture; // Decal texture
	u8 scale; // Pixel scale
	u8 flags; // Decal flags

	i16 entityIndex; // Entity this is attached to
};

#endif // ENGINE_PRIMITIVES_DECAL_HPP
