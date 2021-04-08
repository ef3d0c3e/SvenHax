#include "AntiAim.hpp"
#include "../Interface.hpp"

namespace Settings::AA
{
bool enabled = true;
bool clamp   = false;

Yaw yaw               = Yaw::NONE;
f32 yawOffset         = 0.f;
f32 yawJitterRange[2] = {-30.f, +30.f};
f32 yawSpinSpeed      = 10.f;

Pitch pitch             = Pitch::NONE;
f32 pitchValue          = -89.f;
f32 pitchJitterValue[2] = {-89.f, +89.f};

Roll roll     = Roll::NONE;
f32 rollValue = 0;
}

QAngle AntiAim::previousAngle = QAngle(0.f, 0.f, 0.f);
void AntiAim::CreateMove(UserCmd* cmd)
{
	if (!Settings::AA::enabled)
	{
		previousAngle = QAngle(0, 0, 0);
		return;
	}

	if (!gameEngine->IsInGame())
		return;

	if (!gEngineFuncs->GetLocalPlayer())
		return;

	if (cmd->buttons & IN_USE || cmd->buttons & IN_ATTACK || cmd->buttons & IN_ATTACK2)
		return;

    QAngle oldAngle = cmd->viewAngles;
    f32 oldForward = cmd->forwardMove;
    f32 oldSideMove = cmd->sideMove;

	QAngle angle = previousAngle;

	static bool flip = true;
	//todo: sendpacket, fake up/down won't work without it
	QAngle temp(0, 0, 0);
	// {{{ Yaw
	switch (Settings::AA::yaw)
	{
		case Settings::AA::Yaw::NONE:
			angle[YAW] = oldAngle[YAW];
			break;
		case Settings::AA::Yaw::BACKWARD:
			angle[YAW] = oldAngle[YAW] + 180.f + Settings::AA::yawOffset;
			break;
		case Settings::AA::Yaw::JITTER:
			angle[YAW] = Settings::AA::yawJitterRange[flip] + 180.f + Settings::AA::yawOffset;
			break;
		case Settings::AA::Yaw::SPIN:
			angle[YAW] += Settings::AA::yawSpinSpeed;
			break;
		case Settings::AA::Yaw::LISP:
			angle[YAW] = oldAngle[YAW] + (flip ? 323210000.0f : 323210000.0f) + Settings::AA::yawOffset;
			break;
		case Settings::AA::Yaw::ANGEL:
			angle[YAW] = oldAngle[YAW] + 36000180.0f + Settings::AA::yawOffset;
			break;
		case Settings::AA::Yaw::ANGEL_INVERSE:
			angle[YAW] = 36000180.0f + Settings::AA::yawOffset;
			break;
		case Settings::AA::Yaw::ANGEL_SPIN:
			//angle[YAW] = 36000000.0f + engine->GetCurtu;
			break;
	}
	// }}}
	// {{{ Pitch
	switch (Settings::AA::pitch)
	{
		case Settings::AA::Pitch::NONE:
			angle[PITCH] = oldAngle[PITCH];
			break;
		case Settings::AA::Pitch::STATIC:
			angle[PITCH] = Settings::AA::pitchValue;
			break;
		case Settings::AA::Pitch::JITTER:
			angle[PITCH] = Settings::AA::pitchJitterValue[flip];
			break;
		case Settings::AA::Pitch::FAKE_UP:
			angle[PITCH] = flip ? 89.f : -89.f;
			break;
		case Settings::AA::Pitch::FAKE_DOWN:
			angle[PITCH] = flip ? -89.f : 89.f;
			break;
		case Settings::AA::Pitch::LISP:
			angle[PITCH] = 1800000.f - Settings::AA::pitchValue;
			break;
		case Settings::AA::Pitch::ANGEL_DOWN:
			angle[PITCH] = 36000088.0f;
			break;
		case Settings::AA::Pitch::ANGEL_UP:
			angle[PITCH] = 35999912.0f;
			break;
	}
	// }}}
	// {{{ Roll
	switch (Settings::AA::roll)
	{
		case Settings::AA::Roll::NONE:
			angle[ROLL] = oldAngle[ROLL];
			break;
		case Settings::AA::Roll::STATIC:
			angle[ROLL] = Settings::AA::rollValue;
			break;
	}
	// }}}
	flip = !flip;

	if (Settings::AA::clamp)
	{
        Math::NormalizeAngles(angle);
        Math::ClampAngles(angle);
	}
	previousAngle = angle;
	cmd->viewAngles = angle;


	Math::CorrectMovement(oldAngle, cmd, oldForward, oldSideMove);
}
