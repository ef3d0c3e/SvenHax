#ifndef ENGINE_PRIMITIVES_MSTUDIOMODEL_HPP
#define ENGINE_PRIMITIVES_MSTUDIOMODEL_HPP

#include "Defs.hpp"
#include <array>

struct MStudioModel
{
	static constexpr inline std::size_t NameLen = 64;
	std::array<char, NameLen> name;
	
	i32 type;
	
	f32 boundingradius;
	
	i32 nummesh;
	i32 meshindex;
	
	i32 numVerts; // number of unique vertices
	i32 vertInfoIndex; // vertex bone info
	i32 vertIndex; // vertex vec3_t
	i32 numBorms; // number of unique surface normals
	i32 normInfoIndex; // normal bone info
	i32 normIndex; // normal vec3_t
	
	i32 numGroups; // deformation groups
	i32 groupIndex;
};

#endif // ENGINE_PRIMITIVES_MSTUDIOMODEL_HPP
