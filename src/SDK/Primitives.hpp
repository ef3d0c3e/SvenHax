#ifndef SDK_PRIMITIVES_HPP
#define SDK_PRIMITIVES_HPP

#include "Color.hpp"
#include "Vector.hpp"

struct MVertex
{
	Vec pos;
}

struct MPlane
{
	Vec normal; // surface normal
	float dist; // closest appoach to origin
	u8 type; // for texture axis selection and fast side tests
	u8 signbits; // signx + signy<<1 + signz<<1
	u8 pad[2];
};

struct DModel
{
	Vec mins, maxs;
	Vec origin;
	int headnode[Hull::MaxMapHulls];
	int visleafs; // not including the solid leaf 0
	int firstface, numfaces;
};

struct MNode
{
	// common with leaf
	int contents; // 0, to differentiate from leafs
	int visframe; // node needs to be traversed if current

	Vec mins, maxs;

	MNode* parent;

	// node specific
	MPlane* plane;
	MNode* children[2];

	unsigned short firstsurface;
	unsigned short numsurfaces;
}

struct EFrag
{
	struct MLeaf* leaf;
	EFrag* leafNext;
	Entity* entity;
	EFrag* entNext;
}

struct MLeaf
{
	// common with node
	int contents; // wil be a negative contents number
	int visframe; // node needs to be traversed if current

	Vector<short, 3> mins, maxs;

	MNode* parent;

	// leaf specific
	u8* compressed_vis;
	EFrag* efrags;

	msurface_t** firstmarksurface;
	int nummarksurfaces;
	int key; // BSP sequence number for leaf's contents
	u8 ambient_sound_level[NUM_AMBIENTS];
}

struct MEdge
{
	unsigned short v[2];
	unsigned int cachededgeoffset;
};

struct Texture
{
	static constexpr inline std::size_t NameLen = 16;
	char name[NameLen];
	unsigned width, height;
	int anim_total; // total tenths in sequence ( 0 = no)
	int anim_min, anim_max; // time for this frame min <=time< max
	Texture* anim_next; // in the animation sequence
	Texture* alternate_anims; // bmodels in frame 1 use these
	static constexpr inline std::size_t MipLevels = 4;
	unsigned offsets[MipLevels]; // four mip maps stored
	unsigned paloffset;
};

struct MTexInfo
{
	float vecs[2][4]; // [s/t] unit vectors in world space.
	// [i][3] is the s/t offset relative to the origin.
	// s or t = dot(3Dpoint,vecs[i])+vecs[i][3]
	float mipadjust; // ?? mipmap limits for very small surfaces
	Texture* texture;
	int flags; // sky or slime, no lightmap or 256 subdivision
};

struct Decal
{
	Decal* pnext; // linked list for each surface
	struct MSurface* psurface; // Surface id for persistence / unlinking
	short dx; // Offsets into surface texture (in texture coordinates, so we don't need floats)
	short dy;
	short texture; // Decal texture
	u8 scale; // Pixel scale
	u8 flags; // Decal flags

	short entityIndex; // Entity this is attached to
};

struct MSurface
{
	int visframe; // should be drawn when node is crossed

	int dlightframe; // last frame the surface was checked by an animated light
	int dlightbits; // dynamically generated. Indicates if the surface illumination
	// is modified by an animated light.

	MPlane* plane; // pointer to shared plane
	int flags; // see SURF_ #defines

	int firstedge; // look up in model->surfedges[], negative numbers
	int numedges; // are backwards edges

	// surface generation data
	struct surfcache_s* cachespots[Texture::MipLevels];

	short texturemins[2]; // smallest s/t position on the surface.
	short extents[2]; // ?? s/t texture size, 1..256 for all non-sky surfaces

	MTexInfo* texinfo;

	// lighting info
	static constexpr inline std::size_t MaxLightMaps = 4;
	u8 styles[MaxLightMaps]; // index into d_lightstylevalue[] for animated lights
	// no one surface can be effected by more than 4
	// animated lights.
	Color24* samples;

	decal_t* pdecals;
}

struct DClipNode
{
	int planenum;
	short children[2]; // negative numbers are contents
};

struct Hull
{
	static constexpr inline std::size_t MaxMapHulls = 4;

	DClipNode* clipnodes;
	MPlane* planes;
	int firstclipnode;
	int lastclipnode;
	Vec clip_mins;
	Vec clip_maxs;
};

struct CacheUser
{
	void* data;
};

struct PMPlane
{
	Vec normal;
	float dist;
};

struct PMTrace
{
	qboolean allsolid; // if true, plane is not valid
	qboolean startsolid; // if true, the initial point was in a solid area
	qboolean inopen, inwater; // End point is in empty space or in water
	float fraction; // time completed, 1.0 = didn't hit anything
	Vec endpos; // final position
	PMPlane plane; // surface normal at impact
	int ent; // entity at impact
	Vec deltavelocity; // Change in player's velocity caused by impact.
	// Only run on server.
	int hitgroup;
}

#endif // SDK_PRIMITIVES_HPP
