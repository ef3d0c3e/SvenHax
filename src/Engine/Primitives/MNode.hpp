#ifndef ENGINE_PRIMITIVES_MNODE_HPP
#define ENGINE_PRIMITIVES_MNODE_HPP

#include "Vector.hpp"
#include "MPlane.hpp"

struct MNode
{
	// common with leaf
	i32 contents; // 0, to differentiate from leafs
	i32 visFrame; // node needs to be traversed if current

	Vec3 mins, maxs;

	MNode* parent;

	// node specific
	MPlane* plane;
	MNode* children[2];

	u16 firstSurface;
	u16 numSurfaces;
};

#endif // ENGINE_PRIMITIVES_MNODE_HPP
