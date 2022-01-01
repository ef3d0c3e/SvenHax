#ifndef HACKS_RAINBOWMODEL_HPP
#define HACKS_RAINBOWMODEL_HPP

#include "../Interface.hpp"

namespace RainbowModel
{
	void Paint();
}

namespace Settings::RainbowModel
{
	extern bool top;
	extern f32 topSpeed;
	extern bool bottom;
	extern f32 bottomSpeed;
}

#endif // HACKS_RAINBOWMODEL_HPP
