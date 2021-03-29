#ifndef ENGINE_PRIMITIVES_REFPARAMS_HPP
#define ENGINE_PRIMITIVES_REFPARAMS_HPP

#include "MoveVars.hpp"
#include "Vector.hpp"

struct RefParams
{
	// Output
	Vec3 viewOrg;
	QAngle viewAngles;

	Vec3 forward, right, up;

	// Client frametime;
	f32 frameTime;
	// Client time
	f32 time;

	// Misc
	i32 intermission;
	i32 paused;
	i32 spectator;
	i32 onGround;
	i32 waterLevel;

	Vec3 simVel, simOrg;

	Vec3 viewHeight;
	f32 idealPitch;

	QAngle clViewAngles;

	i32 health;
	QAngle crosshairAngle;
	f32 viewsize;

	QAngle punchAngle;
	i32 maxClients;
	i32 viewEntity;
	i32 playerNum;
	i32 maxEntities;
	i32 demoPlayback;
	i32 hardware;

	i32 smoothing;

	// Last issued usercmd
	UserCmd* cmd;

	// Movevars
	MoveVars* moveVars;

	Vector<i32, 4> viewPort; // // the viewport coordinates x ,y , width, height

	i32 nextView; // the renderer calls ClientDLL_CalcRefdef() and Renderview
		// so long in cycles until this value is 0 (multiple views)
	i32 onlyClientDraw; // if !=0 nothing is drawn by the engine except clientDraw functions
};

#endif // ENGINE_PRIMITIVES_REFPARAMS_HPP
