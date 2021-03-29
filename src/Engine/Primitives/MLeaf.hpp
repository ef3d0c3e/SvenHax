#ifndef ENGINE_PRIMITIVES_MLEAF_HPP
#define ENGINE_PRIMITIVES_MLEAF_HPP

#include "EFrag.hpp"

struct MLeaf
{
	// common with node
	i32 contents; // wil be a negative contents number
	i32 visFrame; // node needs to be traversed if current

	Vector<u16, 3> mins, maxs;

	MNode* parent;

	// leaf specific
	u8* compressedVis;
	EFrag* efrags;

	msurface_t** firstMarkSurface;
	i32 numMarkSurfaces;
	i32 key; // BSP sequence number for leaf's contents
	static constexpr inline NumAmbients = 4;
	u8 ambientSoundLevel[NumAmbients];
}

#endif // ENGINE_PRIMITIVES_MLEAF_HPP
