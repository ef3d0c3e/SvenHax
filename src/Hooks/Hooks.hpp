#ifndef HOOKS_HOOKS_HPP
#define HOOKS_HOOKS_HPP

#include "../SDK/SDK.hpp"

namespace Hooks
{
	// Painting
	void Paint(void* thisptr, PaintMode_t mode); // Draw with Surface
	//void PaintImGui(); // Draw with ImGui.
}

/*
namespace CreateMove
{
	extern bool sendPacket;
	extern QAngle lastTickViewAngles;
}

namespace SetKeyCodeState
{
	extern bool shouldListen;
	extern KeyCode* keyOutput;
}
*/

namespace Paint
{
    // updated in paint.
    extern int engineWidth;
    extern int engineHeight;
    // updated in sdlhook (for stretched users, instead of stretching the whole imgui system, we can do it ourselves and leave the menu crisp)
    extern int windowWidth;
    extern int windowHeight;
}

#endif // HOOKS_HOOKS_HPP
