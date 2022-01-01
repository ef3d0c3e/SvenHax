#ifndef SDK_CSTUDIOMODELRENDERER_HPP
#define SDK_CSTUDIOMODELRENDERER_HPP

#include "../Engine/Primitives/EntityState.hpp"
#include "../Engine/Primitives/MStudioSeqDesc.hpp"
#include "../Engine/Primitives/MStudioAnim.hpp"
#include "../Engine/Primitives/MStudioBone.hpp"
#include "../Engine/Primitives/CVar.hpp"
#include "../Engine/Primitives/PlayerInfo.hpp"
#include "../Engine/Primitives/StudioHDR.hpp"
#include "../Engine/Primitives/MStudioModel.hpp"
#include "../Engine/Primitives/MStudioBodyParts.hpp"
#include "../Engine/Entity.hpp"
#include "../Engine/Model.hpp"

class CStudioModelRenderer
{
public:
	virtual ~CStudioModelRenderer() = 0;

	virtual void Init() = 0;
	virtual i32 StudioDrawModel(i32 flags) = 0;
	virtual i32 StudioDrawPlayer(i32 flags, EntityState* player) = 0;
	virtual i32 StudioDrawMonster(i32 flags, Entity* monster) = 0;
	virtual MStudioAnim StudioGetAnim(Model* subModel, MStudioSeqDesc* seqDes) = 0;
	virtual void StudioSetUpTransform(i32 trivialAccept) = 0;
	virtual void StudioSetupBones() = 0;
	virtual void StudioCalcAttachments() = 0;
	virtual void StudioSaveBones() = 0;
	virtual void StudioMergeBones(Model* subModel) = 0;
	virtual f32 StudioEstimateInterpolant() = 0;
	virtual f32 StudioEstimateFrame(MStudioSeqDesc* seqDesc) = 0;
	virtual f32 /*?*/ StudioEstimateGaitFrame(MStudioSeqDesc* seqDesc) = 0;
	virtual void StudioFxTransform(Entity* ent, Mat3x4 transform) = 0;
	virtual void StudioSlerpBones(Quaternion q1[], Vec3 pos1[], Quaternion q2[], Vec3 pos2[], f32 s) = 0;
	virtual void StudioCalcBoneAdj(f32 dadt, f32* adj, const u8* controller1, const u8* controller2, u8 mouthOpen) = 0;
	virtual void StudioCalcBoneQuaterion(i32 frame, f32 s, MStudioBone* bone, MStudioAnim* anim, f32* adj, const Quaternion& q) = 0; /* Not sure about the const... */
	virtual void StudioCalcBonePosition(f32 frame, f32 s, MStudioBone* bone, MStudioAnim* anim, f32* adj, const Vec3& pos) = 0;
	virtual void StudioCalcRotations(Vec3 pos[], Quaternion q[], MStudioSeqDesc* seqDesc, MStudioAnim* anim, f32 f) = 0;
	virtual void StudioRenderModel() = 0; // draws viewmodel
	virtual void StudioRenderFinal() = 0;
	virtual void StudioRenderFinal_Software() = 0;
	virtual void StudioRenderFinal_Hardware() = 0;
	virtual void StudioMonsterBlend(MStudioSeqDesc* seqDesc, i32* blend, f32* pitch) = 0;
	virtual void StudioEstimateGait(EntityState*) = 0;
	virtual void StudioEstimateMonsterGait(Entity*) = 0;
	virtual void StudioProcessGait(EntityState*) = 0;
	virtual void StudioProcessMonsterGait(Entity*) = 0;

	f64 clkTime; // Client clock
	f64 clkOldTime; // Old Client clock
	
	QBool doInterp; // Do interpolation?
	i32 gaitEstimation; // Do gait estimation?
	
	i32 frameCount; // Current render frame
	
	CVar* cvarHiModels; // Use high quality models?
	CVar* cvarDeveloper; // Developer debug output desired?
	CVar* cvarDrawEntities; // Draw entities bone hit boxes, etc?
	
	Entity* currentEntity; // The entity which we are currently rendering.
	
	Model* renderModel; // The model for the entity being rendered
	
	PlayerInfo* playerInfo; // Player info for current player, if drawing a player
	
	i32 playerIndex; // The index of the player being drawn
	
	f32 gaitMovement; // The player's gait movement
	
	StudioHDR* studioHeader; // Pointer to header block for studio model data
	
	MStudioBodyParts* bodyPart; // Pointer to current body part
	MStudioModel* subModel; // Pointer to submodel
	
	// Palette substition for top and bottom of model
	i32 topColor;
	i32 bottomColor;
	
	Model* chromeSprite; // Sprite model used for drawing studio model chrome
	
	// --- Caching ---
	static constexpr inline std::size_t MaxStudioBones = 128; // Total bones actually used
	i32 cachedBones;  // Number of bones in bone cache
	std::array<std::array<char, MaxStudioBones>, 32> cachedBonesNames; // Names of cached bones
	// Cached bone & light transformation matrices
	std::array<Mat3x4, MaxStudioBones> cachedBoneTransform; // Cached bone transform matrix
	std::array<Mat3x4, MaxStudioBones> cachedLightTransform; // Cached light transform matrix
	
	Vec2 softwareScale; // Software renderer scale factors
	Vec3 up, right, normal; // Current view vectors and render origin
	
	Vec3 renderOrigin;
	
	// Model render counters (from engine)
	i32* studioModelCount;
	i32* modelsDrawn;
	
	// --- Matrices ---
	Mat3x4* rotationMatrix; // Model to world transformation
	Mat3x4* aliasTransform; // Model to view transformation
	
	// Concatenated bone and light transforms
	std::array<Mat3x4*, MaxStudioBones> boneTransform;
	std::array<Mat3x4*, MaxStudioBones> lightTransform;
};

extern CStudioModelRenderer* gStudioRenderer;

#endif // SDK_CSTUDIOMODELRENDERER_HPP
