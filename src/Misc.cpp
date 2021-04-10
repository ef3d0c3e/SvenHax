#include "Misc.hpp"
#include "Engine/Engine.hpp"

namespace Misc
{
i32 mousedx = 0;
i32 mousedy = 0;
}

void Misc::CreateMove(UserCmd* cmd)
{
	static i32 lastx = 0, lasty = 0;

	i32 x, y;
	gEngineFuncs->GetMousePosition(&x, &y);
	mousedx = x - lastx;
	mousedy = y - lasty;
	
	lastx = x, lasty = y;
}
