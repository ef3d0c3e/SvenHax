#ifndef HOOKS_HOOKS_HPP
#define HOOKS_HOOKS_HPP

#include "../SDK/SDK.hpp"

namespace Hooks
{
	// Painting
	//void Paint(void* thisptr, PaintMode_t mode); // Draw with Surface
	void PaintImGui(); // Draw with ImGui.

	//void IsKeyDown(void* thisptr, KeyCode code);
}

/*
namespace CreateMove
{
	extern bool sendPacket;
	extern QAngle lastTickViewAngles;
}

*/
namespace KeyCodeState
{
	extern bool shouldListen;
	extern KeyCode* keyOutput;
}

namespace SDL2
{
	extern int windowWidth;
	extern int windowHeight;

	void HookSwapWindow();
	void UnhookWindow();

	void HookPollEvent();
	void UnhookPollEvent();
}

#endif // HOOKS_HOOKS_HPP
