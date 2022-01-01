#ifndef ENGINE_PRIMITIVES_STUDIOHDR_HPP
#define ENGINE_PRIMITIVES_STUDIOHDR_HPP

#include "Vector.hpp"
#include <array>

struct StudioHDR
{
	i32 id;
	i32 version;
	
	static constexpr inline std::size_t NameLen = 64;
	std::array<char, NameLen> name;
	i32 length;
	
	Vec3 eyePos; // ideal eye position
	Vec3 min; // ideal movement hull size
	Vec3 max;
	
	Vec3 bbmin; // clipping bounding box
	Vec3 bbmax;
	
	i32 flags;
	
	i32 numBones; // bones
	i32 boneIndex;
	
	i32 numBonecontrollers; // bone controllers
	i32 boneControllerIndex;
	
	i32 numHitboxes; // complex bounding boxes
	i32 hitBoxindex;
	
	i32 numSeq; // animation sequences
	i32 seqIndex;
	
	i32 numSeqgroups; // demand loaded sequences
	i32 seqGroupIndex;
	
	i32 numTextures; // raw textures
	i32 textureIndex;
	i32 textureDataIndex;
	
	i32 numSkinRef; // replaceable textures
	i32 numSkinFamilies;
	i32 skinIndex;
	
	i32 numBodyParts;
	i32 bodyPartIndex;
	
	i32 numAttachments; // queryable attachable points
	i32 attachmentIndex;
	
	i32 soundTable;
	i32 soundIndex;
	i32 soundGroups;
	i32 soundGroupIndex;
	
	i32 numTransitions; // animation node to animation node transition graph
	i32 transitionIndex;
};

#endif // ENGINE_PRIMITIVES_STUDIOHDR_HPP
