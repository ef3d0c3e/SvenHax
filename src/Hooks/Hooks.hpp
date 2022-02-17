#ifndef HOOKS_HOOKS_HPP
#define HOOKS_HOOKS_HPP

#include "../SDK/SDK.hpp"
#include "../Interface.hpp"
#include "../Engine/ClientDLL.hpp"
#include <chrono>
#include <vector>

namespace Hooks
{
	// Painting
	void PaintImGui(); // Draw with ImGui.

	//void IsKeyDown(void* thisptr, KeyCode code);
	
	void CreateMove(f32 frameTime, UserCmd* cmd, i32 active);
}

namespace CreateMove
{
	extern bool sendPacket;
	extern QAngle lastViewAngles;

	void HookCreateMove();
	void UnhookCreateMove();
}

namespace KeyCodeState
{
	extern bool shouldListen;
	extern KeyCode* keyOutput;
}

namespace ClientDLL
{
	void HookRedraw();
	void UnhookRedraw();
}

namespace SDL2
{
	extern int windowWidth;
	extern int windowHeight;
	extern std::chrono::time_point<std::chrono::high_resolution_clock> frameBegin;

	void HookSwapWindow();
	void UnhookWindow();

	void HookPollEvent();
	void UnhookPollEvent();
}

namespace StudioModelRenderer
{
	i32 StudioDrawModel(CStudioModelRenderer* thisptr, i32 flags);
	i32 StudioDrawPlayer(CStudioModelRenderer* thisptr, i32 flags, EntityState* player);
	i32 StudioDrawMonster(CStudioModelRenderer* thisptr, i32 flags, Entity* monster);
	void StudioRenderModel(CStudioModelRenderer* thisptr);
}

#endif // HOOKS_HOOKS_HPP
