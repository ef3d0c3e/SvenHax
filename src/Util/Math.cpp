#include "Math.hpp"



std::pair<f32, f32> Math::SinCos(f32 t)
{
	f32 cosr, sinr;
	__asm ("fsincos" : "=t" (cosr), "=u" (sinr) : "0" (t));

	return {sinr, cosr};
}

f32 Math::Deg2Rad(f32 deg)
{
	static constexpr f32 DEG2RAD = M_PI / 180.f;
	return deg * DEG2RAD;
}

f32 Math::Rad2Deg(f32 rad)
{
	static constexpr f32 RAD2DEG = 180.f / M_PI;
	return rad * RAD2DEG;
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

f32 Math::NormalizeYaw(f32 yaw)
{
	while (yaw > 180.f)
		yaw -= 360.f;
	while (yaw < -180.f)
		yaw += 360.f;

	return yaw;
}

void Math::CorrectMovement(const QAngle &vOldAngles, UserCmd* cmd, f32 fOldForward, f32 fOldSidemove)
{
	// side/forward move correction
	f32 deltaView;
	f32 f1;
	f32 f2;

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

QAngle Math::AngleForVec(const Vec3& forward)
{
	QAngle angles;
	if (forward[1] == 0.0f && forward[0] == 0.0f)
	{
		angles[0] = (forward[2] > 0.0f) ? 270.0f : 90.0f; // Pitch (up/down)
		angles[1] = 0.0f;  //yaw left/right
	}
	else
	{
		angles[0] = - Rad2Deg(std::atan2(-forward[2], sqrt(forward[0]*forward[0]+forward[1]*forward[1]) ));
		angles[1] = Rad2Deg(std::atan2(forward[1], forward[0]));

		if (angles[1] > 90)
			angles[1] -= 180;
		else if (angles[1] < 90)
			angles[1] += 180;
		else if (angles[1] == 90)
			angles[1] = 0;
	}

	angles[2] = 0.0f;

	return angles;
}

Vec3 Math::VectorAngle(const QAngle& angles)
{
	const auto&& [sy, cy] = Math::SinCos(Deg2Rad(angles[YAW]));
	const auto&& [sp, cp] = Math::SinCos(Deg2Rad(angles[PITCH]));

	return Vec3(cp*cy, cp*sy, -sp);
}
