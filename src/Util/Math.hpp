#ifndef UTIL_MATH_HPP
#define UTIL_MATH_HPP

#include "../Engine/Primitives/Vector.hpp"
#include "../Engine/Primitives/Color.hpp"
#include "../Engine/Primitives/QAngle.hpp"
#include "../Engine/Primitives/UserCmd.hpp"


namespace Math
{
	std::pair<f32, f32> SinCos(f32 t);
	float Deg2Rad(float deg);
	void NormalizeAngles(QAngle& angle);
	void ClampAngles(QAngle& angle);
	void CorrectMovement(const QAngle &vOldAngles, UserCmd* cmd, float fOldForward, float fOldSidemove);

	QAngle AngleForVec(const Vec3& forward);
}

#endif // UTIL_MATH_HPP
