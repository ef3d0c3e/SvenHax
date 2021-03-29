#ifndef ENNGINE_MODEL_HPP
#define ENNGINE_MODEL_HPP

#include "Primitives/DModel.hpp"
#include "Primitives/MLeaf.hpp"
#include "Primitives/MVertex.hpp"
#include "Primitives/MEdge.hpp"
#include "Primitives/MNode.hpp"
#include "Primitives/MTexInfo.hpp"
#include "Primitives/MSurface.hpp"
#include "Primitives/DClipNode.hpp"
#include "Primitives/Hull.hpp"
#include "Primitives/Texture.hpp"
#include "Primitives/CacheUser.hpp"

MAKE_CENUM_Q(ModelType i32,
	BRUSH, 0,
	SPRITE, 1,
	ALIAS, 2,
	STUDIO 3,
);

MAKE_CENUM_Q(SyncType i32,
	SYNC, 0,
	RAND, 1,
);

struct Model
{
	static constexpr inline std::size_t MaxNameLen = 64;
	char name[MaxNameLen];
	QBool needload; // bmodels and sprites don't cache normally

	ModelType type;
	i32 numframes;
	SyncType synctype;

	i32 flags;

	//
	// volume occupied by the model
	//
	Vec3 mins, maxs;
	f32 radius;

	//
	// brush model
	//
	i32 firstmodelsurface, nummodelsurfaces;

	i32 numsubmodels;
	DModel* submodels;

	i32 numplanes;
	MPlane* planes;

	i32 numleafs; // number of visible leafs, not counting 0
	MLeaf* leafs;

	i32 numvertexes;
	MVertex* vertexes;

	i32 numedges;
	MEdge* edges;

	i32 numnodes;
	MNode* nodes;

	i32 numtexinfo;
	MTexInfo* texinfo;

	i32 numsurfaces;
	MSurface* surfaces;

	i32 numsurfedges;
	i32* surfedges;

	i32 numclipnodes;
	DClipNode* clipnodes;

	i32 nummarksurfaces;
	MSurface** marksurfaces;

	Hull hulls[Hull::MaxMapHulls];

	i32 numtextures;
	Texture** textures;

	u8* visdata;

	Color24* lightdata;

	char* entities;

	//
	// additional model data
	//
	CacheUser cache; // only access through Mod_Extradata
}

#endif // ENNGINE_MODEL_HPP
