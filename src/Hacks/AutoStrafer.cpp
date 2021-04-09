#include "AutoStrafer.hpp"

namespace Settings::AutoStrafer
{
	bool enabled  = true;
	bool silent   = true;
	bool ground   = true;
	bool sideways = true;
}

/*
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
}*/

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
		if (leftRight /*|| cmd->mousedx > 1*/)
		{
			viewAngles[YAW] += yaw_change;
			cmd->sideMove = 250.0f;
		}
		else if (!leftRight /*|| cmd->mousedx < 1*/)
		{
			viewAngles[YAW] -= yaw_change;
			cmd->sideMove = -250.0f;
		}

		leftRight = !leftRight;
	}

	Math::NormalizeAngles(viewAngles);
	Math::ClampAngles(viewAngles);

	Math::CorrectMovement(viewAngles, cmd, cmd->forwardMove, cmd->sideMove);

	if (!Settings::AutoStrafer::silent)
		cmd->viewAngles = viewAngles;

}

static void AirStrafe(UserCmd* cmd)
{
	if (gPMove->flags & MoveFlags::ONGROUND || cmd->buttons & IN_USE || cmd->buttons & IN_ATTACK || cmd->buttons & IN_ATTACK2)
		return;



}

void AutoStrafer::CreateMove(UserCmd* cmd)
{
	if (!Settings::AutoStrafer::enabled)
		return;

	if (!gameEngine->IsInGame())
		return;

	if (gPMove->moveType == MoveType::FLY || gPMove->moveType == MoveType::NOCLIP)
		return;

	AirStrafe(cmd);
	//RageStrafe(cmd);

	//if (Settings::AutoStrafer::groundStrafe)
		//GroundStrafe(cmd);
}
