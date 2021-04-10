#ifndef UTIL_MATH_HPP
#define UTIL_MATH_HPP

#include "../Engine/Primitives/Vector.hpp"
#include "../Engine/Primitives/Color.hpp"
#include "../Engine/Primitives/QAngle.hpp"
#include "../Engine/Primitives/UserCmd.hpp"


namespace Math
{
	std::pair<f32, f32> SinCos(f32 t);
	f32 Deg2Rad(f32 deg);
	f32 Rad2Deg(f32 rad);
	void NormalizeAngles(QAngle& angle);
	f32 NormalizeYaw(f32 yaw);
	void ClampAngles(QAngle& angle);
	void CorrectMovement(const QAngle &vOldAngles, UserCmd* cmd, f32 fOldForward, f32 fOldSidemove);

	QAngle AngleForVec(const Vec3& forward);
	Vec3 VectorAngle(const QAngle& angles);
}

#endif // UTIL_MATH_HPP
