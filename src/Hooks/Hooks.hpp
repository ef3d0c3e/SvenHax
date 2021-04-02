#ifndef HOOKS_HOOKS_HPP
#define HOOKS_HOOKS_HPP

#include "../SDK/SDK.hpp"
#include "../Engine/ClientDLL.hpp"

namespace Hooks
{
	// Painting
	void PaintImGui(); // Draw with ImGui.

	//void IsKeyDown(void* thisptr, KeyCode code);
	
	void CreateMove(f32 frameTime, UserCmd* cmd, i32 active);
}

namespace CreateMove
{
	void HookCreateMove();
	void UnhookCreateMove();
}

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
