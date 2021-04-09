#ifndef HACKS_AUTOSTRAFER_HPP
#define HACKS_AUTOSTRAFER_HPP

#include "../Interface.hpp"

namespace AutoStrafer
{
	void CreateMove(UserCmd* cmd);
}

namespace Settings::AutoStrafer
{
	extern bool enabled;
	extern bool silent;
	extern bool ground;
	extern bool sideways;
}

#endif // HACKS_AUTOSTRAFER_HPP
