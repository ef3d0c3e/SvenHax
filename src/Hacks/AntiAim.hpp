#ifndef HACKS_ANTIAIM_HPP
#define HACKS_ANTIAIM_HPP

#include "../Engine/Primitives/QAngle.hpp"
#include "../Engine/Primitives/UserCmd.hpp"

namespace AntiAim
{
	extern QAngle previousAngle;

	void CreateMove(UserCmd* cmd);
}

#endif // HACKS_ANTIAIM_HPP
