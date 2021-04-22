#include "ESP.hpp"
#include "../Engine/Primitives/Matrix.hpp"

namespace Settings::ESP
{
	bool enabled = true;
}

Mat3x3 viewMat;

bool ESP::WorldToScreen(Vec3& point, ImVec2& screen)
{
	return gEngineFuncs->pTriAPI->WorldToScreen((float*)&point, (float*)&screen);
}

void ESP::Paint()
{
	if (!Settings::ESP::enabled)
		return;

	if (!gameEngine->IsInGame())
		return;

	static bool first = true;
	if (first)
		std::cout << "f\n";
	first = false;

	/*for (i32 i = 1; i < *entNum; ++i)
	{
		//std::cout << i << " : " << entList[i].origin << "\n";
	}*/
	gEngineFuncs->pTriAPI->Brightness(25500.f);
}


