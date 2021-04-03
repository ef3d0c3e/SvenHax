#include "AntiAim.hpp"
#include "../Interface.hpp"

QAngle AntiAim::previousAngle = QAngle(0.f, 0.f, 0.f);
void AntiAim::CreateMove(UserCmd* cmd)
{
	if (!gameEngine->IsInGame())
		return;

	//engine->GetUserEntity();

    QAngle oldAngle = cmd->viewAngles;
    float oldForward = cmd->forwardmove;
    float oldSideMove = cmd->sidemove;

	QAngle angle = cmd->viewAngles;

	angle[1] += 35;

	Math::NormalizeAngles(angle);

	cmd->viewAngles = angle;

    Math::CorrectMovement(oldAngle, cmd, oldForward, oldSideMove);
}
