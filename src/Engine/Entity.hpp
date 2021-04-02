#ifndef ENGINE_ENTITY_HPP
#define ENGINE_ENTITY_HPP

#include "Model.hpp"
#include "Primitives/EntityState.hpp"
#include "Primitives/PositionHistory.hpp"
#include "Primitives/Mouth.hpp"
#include "Primitives/LatchedVars.hpp"

struct Entity
{
	i32 index; // Index into cl_entities ( should match actual slot, but not necessarily )

	QBool player; // True if this entity is a "player"

	EntityState baseline; // The original state from which to delta during an uncompressed message
	EntityState prevstate; // The state information from the penultimate message received from the server
	EntityState curstate; // The state information from the last message received from server

	i32 currentPosition; // Last received history update index
	static constexpr inline std::size_t PositionHistoryMax = 64;
	PositionHistory positionHistory[PositionHistoryMax]; // History of position and angle updates for this player

	Mouth mouth; // For synchronizing mouth movements.

	LatchedVars latched; // Variables used by studio model rendering routines

	// Information based on interplocation, extrapolation, prediction, or just copied from last msg received.
	//
	f32 lastMove;

	// Actual render position and angles
	Vec3 origin;
	QAngle angles;

	// Attachment points
	Vec3 attachment[4];

	// Other entity local information
	i32 trivialAccept;

	Model* model; // cl.model_precache[ curstate.modelindes ];  all visible entities have a model
	EFrag* efrag; // linked list of efrags
	MNode* topnode; // for bmodels, first world node that splits bmodel, or NULL if not split

	f32 syncbase; // for client-side animations -- used by obsolete alias animation system, remove?
	i32 visframe; // last frame this entity was found in an active leaf
	//colorVec cvFloorColor;
	Vector<i32, 4> cvFloorColor; // ??
};

#endif // ENGINE_ENTITY_HPP
