#ifndef SDK_ENTITY_HPP
#define SDK_ENTITY_HPP

#include "../Util/Cenum.hpp"
#include "Color.hpp"
#include "EngineSurface.hpp" // qboolean
#include "QAngle.hpp"
#include "Vector.hpp"

// {{{ EntityState
MAKE_CENUM_Q(EntityType, int, NORMAL, 1 << 0, BEAM, 1 << 1, );

struct EntityState
{
	// Fields which are filled in by routines outside of delta compression
	EntityType type;
	// Index into cl_entities array for this entity.
	int number;
	float msg_time;

	// Message number last time the player/entity state was updated.
	int messagenum;

	// Fields which can be transitted and reconstructed over the network stream
	Vec origin;
	Vec angles;

	int modelindex;
	int sequence;
	float frame;
	int colormap;
	short skin;
	short solid;
	int effects;
	float scale;

	u8 eflags;

	// Render information
	int rendermode;
	int renderamt;
	Color24 rendercolor;
	int renderfx;

	int movetype;
	float animtime;
	float framerate;
	int body;
	u8 controller[4];
	u8 blending[4];
	Vec velocity;

	// Send bbox down to client for use during prediction.
	Vec mins;
	Vec maxs;

	int aiment;
	// If owned by a player, the index of that player ( for projectiles ).
	int owner;

	// Friction, for prediction.
	float friction;
	// Gravity multiplier
	float gravity;

	// PLAYER SPECIFIC
	int team;
	int playerclass;
	int health;
	qboolean spectator;
	int weaponmodel;
	int gaitsequence;
	// If standing on conveyor, e.g.
	Vec basevelocity;
	// Use the crouched hull, or the regular player hull.
	int usehull;
	// Latched buttons last time state updated.
	int oldbuttons;
	// -1 = in air, else pmove entity number
	int onground;
	int iStepLeft;
	// How fast we are falling
	float flFallVelocity;

	float fov;
	int weaponanim;

	// Parametric movement overrides
	Vec startpos;
	Vec endpos;
	float impacttime;
	float starttime;

	// For mods
	int iuser1;
	int iuser2;
	int iuser3;
	int iuser4;
	float fuser1;
	float fuser2;
	float fuser3;
	float fuser4;
	Vec vuser1;
	Vec vuser2;
	Vec vuser3;
	Vec vuser4;
};
// }}}

// {{{ PositionHistory
struct PositionHistory
{
	// Time stamp for this movement
	float animtime;

	Vec origin;
	Vec angles;
};
// }}}

// {{{ Mouth
struct Mouth
{
	u8 mouthopen; // 0 = mouth closed, 255 = mouth agape
	u8 sndcount; // counter for running average
	int sndavg; // running average
};
// }}}

// {{{ LatchedVars
struct LatchedVars
{
	float prevanimtime;
	float sequencetime;
	u8 prevseqblending[2];
	Vec prevorigin;
	Vec prevangles;

	int prevsequence;
	float prevframe;

	u8 prevcontroller[4];
	u8 prevblending[2];
};
// }}}

// {{{ Entity
struct Entity
{
	int index; // Index into cl_entities ( should match actual slot, but not necessarily )

	qboolean player; // True if this entity is a "player"

	EntityState baseline; // The original state from which to delta during an uncompressed message
	EntityState prevstate; // The state information from the penultimate message received from the server
	EntityState curstate; // The state information from the last message received from server

	int current_position; // Last received history update index
	static constexpr inline std::size_t HistoryMax = 64;
	PositionHistory ph[HistoryMax]; // History of position and angle updates for this player

	Mouth mouth; // For synchronizing mouth movements.

	LatchedVars latched; // Variables used by studio model rendering routines

	// Information based on interplocation, extrapolation, prediction, or just copied from last msg received.
	//
	float lastmove;

	// Actual render position and angles
	Vec origin;
	Vec angles;

	// Attachment points
	Vec attachment[4];

	// Other entity local information
	int trivial_accept;

	struct Model* model; // cl.model_precache[ curstate.modelindes ];  all visible entities have a model
	struct EFrag* efrag; // linked list of efrags
	struct MNode* topnode; // for bmodels, first world node that splits bmodel, or NULL if not split

	float syncbase; // for client-side animations -- used by obsolete alias animation system, remove?
	int visframe; // last frame this entity was found in an active leaf
	//colorVec cvFloorColor;
	Vector<unsgined, 4> cvFloorColor; // ??
}
// }}}

#endif // SDK_ENTITY_HPP
