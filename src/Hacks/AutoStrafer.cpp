#include "AutoStrafer.hpp"
#include "../Misc.hpp"

namespace Settings::AutoStrafer
{
	bool enabled  = true;
	bool silent   = true;
	bool ground   = true;
	bool sideways = true;
}

static constexpr f32 Speed = 300;

static void GroundStrafe(UserCmd* cmd)
{
	static bool state = false;

	if (!state && gPMove->flags & MoveFlags::ONGROUND)
	{
		cmd->buttons |= IN_DUCK;
		state = true;
	}
	else if (state)
	{
		cmd->buttons &= ~IN_DUCK;
		state = false;
	}
}

static void RageStrafe(UserCmd* cmd)
{
	static bool leftRight;
	bool inMove = cmd->buttons & IN_FORWARD || cmd->buttons & IN_BACK || cmd->buttons & IN_MOVELEFT || cmd->buttons & IN_MOVERIGHT;

	const f32 vel = gPMove->velocity.Length<f32>();
	if (cmd->buttons & IN_FORWARD && vel <= 50.0f)
		cmd->forwardMove = 250.0f;

	float yaw_change = 0.0f;
	if (vel > 50.f)
		yaw_change = 30.0f * fabsf(30.0f / vel);

	QAngle viewAngles;
	gEngineFuncs->GetViewAngles(reinterpret_cast<float*>(&viewAngles));

	if (!(gPMove->flags & MoveFlags::ONGROUND) && !inMove)
	{
		if (leftRight || Misc::mousedx > 1)
		{
			viewAngles[YAW] += yaw_change;
			cmd->sideMove = 400.0f;
		}
		else if (!leftRight || Misc::mousedx < 1)
		{
			viewAngles[YAW] -= yaw_change;
			cmd->sideMove = -400.0f;
		}

		leftRight = !leftRight;
	}

	Math::NormalizeAngles(viewAngles);
	Math::ClampAngles(viewAngles);

	Math::CorrectMovement(viewAngles, cmd, cmd->forwardMove, cmd->sideMove);

	if (!Settings::AutoStrafer::silent)
		cmd->viewAngles = viewAngles;

}

inline static f32 get_delta(float speed)
{
	auto term = (30.0 - (gPMove->moveVars->airAccelerate * gPMove->maxSpeed / 66.0)) / speed;

	if (term < 1.0f && term > -1.0f)
	{
		return acos(term);
	}

	return 0.f;
}

static void AirStrafe(UserCmd* cmd)
{
}

void AutoStrafer::CreateMove(UserCmd* cmd)
{
	if (!Settings::AutoStrafer::enabled)
		return;

	if (!gameEngine->IsInGame())
		return;

	if (gPMove->moveType == MoveType::FLY || gPMove->moveType == MoveType::NOCLIP)
		return;

	if (cmd->buttons & IN_USE || cmd->buttons & IN_ATTACK || cmd->buttons & IN_ATTACK2)
		return;


	AirStrafe(cmd);
	//RageStrafe(cmd);

	//if (Settings::AutoStrafer::groundStrafe)
		//GroundStrafe(cmd);
}
