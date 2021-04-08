#include "AntiAim.hpp"
#include "../Interface.hpp"

namespace Settings::AA
{
bool enabled = true;
bool clamp   = false;
Yaw yaw      = Yaw::NONE;
Pitch pitch  = Pitch::NONE;
float yawOffset = 180.f;
float yawJitterRange[2] = {-30.f, +30.f};
}

QAngle AntiAim::previousAngle = QAngle(0.f, 0.f, 0.f);
void AntiAim::CreateMove(UserCmd* cmd)
{
	if (!Settings::AA::enabled)
		return;

	if (!gameEngine->IsInGame())
		return;

	if (!gEngineFuncs->GetLocalPlayer())
		return;

	if (cmd->buttons & IN_USE || cmd->buttons & IN_ATTACK || cmd->buttons & IN_ATTACK2)
		return;

    QAngle oldAngle = cmd->viewAngles;
    f32 oldForward = cmd->forwardMove;
    f32 oldSideMove = cmd->sideMove;

	cmd->viewAngles += QAngle(30, 0, 0);

    Math::CorrectMovement(oldAngle, cmd, oldForward, oldSideMove);
}
