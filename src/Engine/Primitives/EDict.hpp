#ifndef ENGINE_PRIMITIVES_EDICT_HPP
#define ENGINE_PRIMITIVES_EDICT_HPP

#include "Defs.hpp"
#include "Link.hpp"
#include "EntVars.hpp"

struct EDict
{
	QBool free;
	i32 serialNumber;
	Link area; // linked to a division node or leaf

	i32 headNode; // -1 to use normal leaf check
	i32 numLeafs;
	static constexpr inline std::size_t MaxEntLeafs = 48;
	i16 leafNums[MaxEntLeafs];

	f32 freeTime; // sv.time when the object was freed

	void* privateData; // Alloced and freed by engine, used by DLLs

	EntVars vars; // C exported fields from progs

	// other fields from progs come immediately after
};

extern std::array<EDict, 257>* gEnt;
static constexpr inline std::size_t EDictEntsNum = 2048;

#endif // ENGINE_PRIMITIVES_EDICT_HPP
