#ifndef ENGINE_PRIMITIVES_MSTUDIOSEQDESC_HPP
#define ENGINE_PRIMITIVES_MSTUDIOSEQDESC_HPP

#include "Vector.hpp"
#include <array>

struct MStudioSeqDesc
{
	static constexpr inline std::size_t LabelLen = 32;
	std::array<char, LabelLen> label; // sequence label
	
	f32 fps;// frames per second
	i32 flags;// looping/non-looping flags
	
	i32 activity;
	i32 actWeight;
	
	i32 numEvents;
	i32 eventIndex;
	
	i32 numFrames;// number of frames per sequence
	
	i32 numPivots;// number of foot pivots
	i32 pivotIndex;
	
	i32 motionType;
	i32 motionBone;
	Vec3 linearMovement;
	i32 autoMovePosIndex;
	i32 autoMoveAngleIndex;
	
	Vec3 min;// per sequence bounding box
	Vec3 max;
	
	i32 numBlends;
	i32 animIndex;// mstudioanim_t pointer relative to start of sequence group data
	// [blend][bone][X, Y, Z, XR, YR, ZR]
	
	std::array<i32, 2> blendType; // X, Y, Z, XR, YR, ZR
	std::array<f32, 2> blendStart; // starting value
	std::array<f32, 2> blendEnd; // ending value
	i32 blendParent;
	
	i32 seqGroup;// sequence group for demand loading
	
	i32 entryNode;// transition node at entry
	i32 exitNode;// transition node at exit
	i32 nodeFlags;// transition rules
	
	i32 nextSeq;// auto advancing sequences
};

#endif // ENGINE_PRIMITIVES_MSTUDIOSEQDESC_HPP
