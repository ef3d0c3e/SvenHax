#include "RainbowModel.hpp"

namespace Settings::RainbowModel
{
	bool top = true;
	f32 topSpeed = 1.f;
	bool bottom = true;
	f32 bottomSpeed = 2.f;
}

void RainbowModel::Paint()
{
	if (!gameEngine->IsInGame())
		return;
	if (!Settings::RainbowModel::top && !Settings::RainbowModel::bottom)
		return;

	static f32 topColor = 0.f;
	static f32 bottomColor = 0.f;
	static auto hw = reinterpret_cast<u8*>(symbols["hw.so"s].address);

	if (Settings::RainbowModel::top)
		*(hw + 0x1BF02F4) = static_cast<u8>(topColor);
	if (Settings::RainbowModel::bottom)
		*(hw + 0x1BF02F8) = static_cast<u8>(bottomColor);
	topColor = fmod(topColor+Settings::RainbowModel::topSpeed, 255.f);
	bottomColor = fmod(bottomColor+Settings::RainbowModel::bottomSpeed, 255.f);
}
