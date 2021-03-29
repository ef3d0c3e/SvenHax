#ifndef ENGINE_PRIMITIVES_HULL_HPP
#define ENGINE_PRIMITIVES_HULL_HPP

#include "DClipNode.hpp"
#include "MPlane.hpp"


struct Hull
{
	static constexpr inline std::size_t MaxMapHulls = 4;

	DClipNode* clipNodes;
	MPlane* planes;
	i32 firstClipNode;
	i32 lastClipNode;
	Vec3 clipMins;
	Vec3 clipMaxs;
};

#endif // ENGINE_PRIMITIVES_HULL_HPP
