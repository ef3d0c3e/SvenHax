#ifndef ENGINE_PRIMITIVES_MTEXINFO_HPP
#define ENGINE_PRIMITIVES_MTEXINFO_HPP

#include "Texture.hpp"
#include "Matrix.hpp"

struct MTexInfo
{
	Mat4x2 vecs;
	//f32 vecs[2][4]; // [s/t] unit vectors in world space.
	// [i][3] is the s/t offset relative to the origin.
	// s or t = dot(3Dpoint,vecs[i])+vecs[i][3]
	f32 mipadjust; // ?? mipmap limits for very small surfaces
	Texture* texture;
	i32 flags; // sky or slime, no lightmap or 256 subdivision
};

#endif // ENGINE_PRIMITIVES_MTEXINFO_HPP
