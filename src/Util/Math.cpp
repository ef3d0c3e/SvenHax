#include "Math.hpp"



std::pair<f32, f32> Math::SinCos(f32 t)
{
	f32 cosr, sinr;
	__asm ("fsincos" : "=t" (cosr), "=u" (sinr) : "0" (t));

	return {sinr, cosr};
}

float Math::Deg2Rad(float deg)
{
	return deg / 180.f * M_PI;
}

void Math::ClampAngles(QAngle& angle)
{
	if (angle[1] > 180.0f)
		angle[1] = 180.0f;
	else if (angle[1] < -180.0f)
		angle[1] = -180.0f;

	if (angle[0] > 89.0f)
		angle[0] = 89.0f;
	else if (angle[0] < -89.0f)
		angle[0] = -89.0f;

	angle[2] = 0;
}

void Math::NormalizeAngles(QAngle& angle)
{
	while (angle[0] > 89.0f)
		angle[0] -= 180.f;

	while (angle[0] < -89.0f)
		angle[0] += 180.f;

	while (angle[1] > 180.f)
		angle[1] -= 360.f;

	while (angle[1] < -180.f)
		angle[1] += 360.f;
}

void Math::CorrectMovement(const QAngle &vOldAngles, UserCmd* cmd, float fOldForward, float fOldSidemove)
{
	// side/forward move correction
	float deltaView;
	float f1;
	float f2;

	if (vOldAngles[1] < 0.f)
		f1 = 360.0f + vOldAngles[1];
	else
		f1 = vOldAngles[1];

	if (cmd->viewAngles[1] < 0.0f)
		f2 = 360.0f + cmd->viewAngles[1];
	else
		f2 = cmd->viewAngles[1];

	if (f2 < f1)
		deltaView = abs(f2 - f1);
	else
		deltaView = 360.0f - abs(f1 - f2);

	deltaView = 360.0f - deltaView;

	cmd->forwardMove = cos(Deg2Rad(deltaView)) * fOldForward + cos(Deg2Rad(deltaView + 90.f)) * fOldSidemove;
	cmd->sideMove = sin(Deg2Rad(deltaView)) * fOldForward + sin(Deg2Rad(deltaView + 90.f)) * fOldSidemove;
}
