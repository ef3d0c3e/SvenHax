#ifndef MISC_HPP
#define MISC_HPP

#include "Engine/Primitives/Defs.hpp"
#include "Engine/Primitives/UserCmd.hpp"

namespace Misc
{
	extern i32 mousedx;
	extern i32 mousedy;

	void CreateMove(UserCmd* cmd);
}

#endif // MISC_HPP
