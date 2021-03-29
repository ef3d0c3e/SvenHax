#ifndef ENGINE_PRIMITIVES_MSURFACE_HPP
#define ENGINE_PRIMITIVES_MSURFACE_HPP

#include "Decal.hpp"
#include "MTexInfo.hpp"
#include "SurfCache"

struct MSurface
{
	i32 visframe; // should be drawn when node is crossed

	i32 dlightframe; // last frame the surface was checked by an animated light
	i32 dlightbits; // dynamically generated. Indicates if the surface illumination
	// is modified by an animated light.

	MPlane* plane; // pointer to shared plane
	i32 flags; // see SURF_ #defines

	i32 firstedge; // look up in model->surfedges[], negative numbers
	i32 numedges; // are backwards edges

	// surface generation data
	SurfCache* cachespots[Texture::MipLevels];

	i16 texturemins[2]; // smallest s/t position on the surface.
	i16 extents[2]; // ?? s/t texture size, 1..256 for all non-sky surfaces

	MTexInfo* texinfo;

	// lighting info
	static constexpr inline std::size_t MaxLightMaps = 4;
	u8 styles[MaxLightMaps]; // index into d_lightstylevalue[] for animated lights
	// no one surface can be effected by more than 4
	// animated lights.
	Color24* samples;

	Decal* pdecals;
}

#endif // ENGINE_PRIMITIVES_MSURFACE_HPP
