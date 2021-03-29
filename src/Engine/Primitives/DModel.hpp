#ifndef ENGINE_PRIMITIVES_DMODEL_HPP
#define ENGINE_PRIMITIVES_DMODEL_HPP

#include "Hull.hpp"

struct DModel
{
	Vec3 mins, maxs;
	Vec3 origin;
	i32 headNode[Hull::MaxMapHulls];
	i32 visLeafs; // not including the solid leaf 0
	i32 firstFace
	i32 numFaces;
};

#endif // ENGINE_PRIMITIVES_DMODEL_HPP
