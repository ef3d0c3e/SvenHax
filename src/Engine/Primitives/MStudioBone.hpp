#ifndef ENGINE_PRIMITIVES_MSTUDIOBONE_HPP
#define ENGINE_PRIMITIVES_MSTUDIOBONE_HPP

#include "Defs.hpp"

struct MStudioBone
{
	static constexpr inline std::size_t NameLen = 32;
	char name[NameLen]; // bone name for symbolic links
	i32 parent; // parent bone
	i32 flags; // ??
	i32 boneController[6]; // bone controller index, -1 == none
	f32 value[6]; // default DoF values
	f32 scale[6]; // scale for delta DoF values
};

#endif // ENGINE_PRIMITIVES_MSTUDIOBONE_HPP
