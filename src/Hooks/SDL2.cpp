#ifndef HOOKS_SDL2_HPP
#define HOOKS_SDL2_HPP

#include "../ImGUI/imgui.h"
#include "../ImGUI/examples/imgui_impl_opengles3.h"
#include "../ImGUI/examples/libs/gl3w/GL/gl3w.h"
#include "../ImGUI/imgui_internal.h" // for 	ImGui::GetCurrentContext()->Font->DisplayOffset
#include "../Interface.hpp"
#include "../Settings.hpp"
#include "../Shortcuts.hpp"
#include "../UI/UI.hpp"
#include "../Util/Hooker.hpp"
#include "../Util/Util.hpp"
#include "Hooks.hpp"
#include <SDL2/SDL.h>
#include <sys/mman.h>

int SDL2::windowWidth = 0;
int SDL2::windowHeight = 0;


typedef void (*SDL_GL_SwapWindow_t)(SDL_Window*);
Tramp* swapWindowHook = nullptr;
static u8 hookSwapWindowPatch[4] = { 0, 0, 0, 0 };

typedef int (*SDL_PollEvent_t)(SDL_Event*);
Tramp* pollEventHook = nullptr;
static u8 hookPollEventPatch[4] = { 0, 0, 0, 0 };


static void HandleSDLEvent(SDL_Event* event)
{
	ImGuiIO& io = ImGui::GetIO();
	switch (event->type)
	{
		case SDL_MOUSEWHEEL:
			if (event->wheel.y > 0)
				io.MouseWheel = 1;
			if (event->wheel.y < 0)
				io.MouseWheel = -1;

			return;
		case SDL_MOUSEBUTTONDOWN:
			if (event->button.button == SDL_BUTTON_LEFT)
				io.MouseDown[0] = true;
			if (event->button.button == SDL_BUTTON_RIGHT)
				io.MouseDown[1] = true;
			if (event->button.button == SDL_BUTTON_MIDDLE)
				io.MouseDown[2] = true;

			return;
		case SDL_MOUSEBUTTONUP:
			if (event->button.button == SDL_BUTTON_LEFT)
				io.MouseDown[0] = false;
			if (event->button.button == SDL_BUTTON_RIGHT)
				io.MouseDown[1] = false;
			if (event->button.button == SDL_BUTTON_MIDDLE)
				io.MouseDown[2] = false;

			return;
		case SDL_TEXTINPUT:
			io.AddInputCharactersUTF8(event->text.text);

			return;
		case SDL_KEYDOWN:
		case SDL_KEYUP:
			int key = event->key.keysym.sym & ~SDLK_SCANCODE_MASK;
			io.KeysDown[key] = (event->type == SDL_KEYDOWN);
			io.KeyShift = ((SDL_GetModState() & KMOD_SHIFT) != 0);
			io.KeyCtrl = ((SDL_GetModState() & KMOD_CTRL) != 0);
			io.KeyAlt = ((SDL_GetModState() & KMOD_ALT) != 0);
			io.KeySuper = ((SDL_GetModState() & KMOD_GUI) != 0);

			return;
	}
}

static void SwapWindow(SDL_Window* window)
{

	static bool bFirst = true;
	if (bFirst)
	{
		// This sets the opengl function pointers and stuff
		gl3wInit();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();

		// Fixup some keycodes for SDL
		io.KeyMap[ImGuiKey_Tab] = SDLK_TAB;
		io.KeyMap[ImGuiKey_LeftArrow] = SDL_SCANCODE_LEFT;
		io.KeyMap[ImGuiKey_RightArrow] = SDL_SCANCODE_RIGHT;
		io.KeyMap[ImGuiKey_UpArrow] = SDL_SCANCODE_UP;
		io.KeyMap[ImGuiKey_DownArrow] = SDL_SCANCODE_DOWN;
		io.KeyMap[ImGuiKey_PageUp] = SDL_SCANCODE_PAGEUP;
		io.KeyMap[ImGuiKey_PageDown] = SDL_SCANCODE_PAGEDOWN;
		io.KeyMap[ImGuiKey_Home] = SDL_SCANCODE_HOME;
		io.KeyMap[ImGuiKey_End] = SDL_SCANCODE_END;
		io.KeyMap[ImGuiKey_Delete] = SDLK_DELETE;
		io.KeyMap[ImGuiKey_Backspace] = SDLK_BACKSPACE;
		io.KeyMap[ImGuiKey_Enter] = SDLK_RETURN;
		io.KeyMap[ImGuiKey_Escape] = SDLK_ESCAPE;
		io.KeyMap[ImGuiKey_A] = SDLK_a;
		io.KeyMap[ImGuiKey_C] = SDLK_c;
		io.KeyMap[ImGuiKey_V] = SDLK_v;
		io.KeyMap[ImGuiKey_X] = SDLK_x;
		io.KeyMap[ImGuiKey_Y] = SDLK_y;
		io.KeyMap[ImGuiKey_Z] = SDLK_z;

		ImGui_ImplOpenGL3_Init("#version 100");

		bFirst = false;
	}

	ImGuiIO& io = ImGui::GetIO();
	// Setup display size (every frame to accommodate for window resizing)
	int w, h;
	//int display_w, display_h;
	SDL_GetWindowSize(window, &w, &h);
	//SDL_GL_GetDrawableSize(window, &display_w, &display_h);
	SDL2::windowWidth = w;
	SDL2::windowHeight = h;
	io.DisplaySize = ImVec2((float)w, (float)h);
	//io.DisplayFramebufferScale = ImVec2( 1, 1 );

	ImGui_ImplOpenGL3_NewFrame();

	static double lastTime = 0.0f;
	Uint32 time = SDL_GetTicks();
	double currentTime = time / 1000.0;
	io.DeltaTime = lastTime > 0.0 ? (float)(currentTime - lastTime) : (float)(1.0f / 60.0f);

	io.MouseDrawCursor = UI::isVisible && gameEngine->IsInGame();
	io.WantCaptureMouse = UI::isVisible;
	io.WantCaptureKeyboard = UI::isVisible;

	/*
	if (UI::isVisible)
	{
		SDL_Event event;

		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
				return;

			HandleSDLEvent(&event);
		}
	}*/

	if (io.WantCaptureMouse)
	{
		int mx, my;
		SDL_GetMouseState(&mx, &my);

		io.MousePos = ImVec2((float)mx, (float)my);

		SDL_ShowCursor(io.MouseDrawCursor ? 0 : 1);
	}

	ImGui::NewFrame();
	ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
	ImGui::SetNextWindowSize(ImVec2(w, h), ImGuiCond_Always);
	ImGui::SetNextWindowBgAlpha(0.0f);
	ImGui::Begin("", (bool*)true, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoInputs);
	/*
	UI::SetupColors();
	UI::SetupWindows();
	UI::DrawImWatermark();
	 * TODO...
	*/

	if (UI::isVisible)
	ImGui::ShowDemoWindow();

	Hooks::PaintImGui(); // Process ImGui Draw Commands

	ImGui::End();
	ImGui::EndFrame();

	ImGui::GetCurrentContext()->Font->DisplayOffset = ImVec2(0.f, 0.f);
	ImGui::GetCurrentContext()->Font->DisplayOffset = ImVec2(0.0f, 0.0f);

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	swapWindowHook->GetOriginalFunction<SDL_GL_SwapWindow_t>()(window);
	return;
}

void SDL2::HookSwapWindow()
{
	
	// 53                    - push rbx
	// 83 EC 18              - sub esp,18
	// E8 F28FF7FF           - call libSDL2-2.0.so.0+A6F6 <- moves the address of the next instruction into rbx
	// 81 C3 A4380400        - add ebx,000438A4
	// 8B 44 24 20           - mov eax,[rsp+20] 
	// 8B 93 24860000        - mov edx,[rbx+00008624]
	// 85 D2                 - test edx,edx
	swapWindowHook = new Tramp(GetSymbolAddress<std::uintptr_t>("libSDL2-2.0.so.0", "SDL_GL_SwapWindow"), reinterpret_cast<std::uintptr_t>(&SwapWindow), 9,
	[](std::uintptr_t addr, std::uintptr_t gateway)
	{
		*reinterpret_cast<u8*>(gateway+4) = 0xBB; // mov into ebx
		for (std::size_t i = 0; i < 4; ++i)
		{
			hookSwapWindowPatch[i] = *reinterpret_cast<u8*>(gateway+5+i); // Store for later
			*reinterpret_cast<u8*>(gateway+5+i) = ((addr+9) >> 8*i) & 0xFF;
		}
	});
}

void SDL2::UnhookWindow()
{
	swapWindowHook->Release([](std::uintptr_t addr, std::uintptr_t gateway)
	{
		*reinterpret_cast<u8*>(addr+4) = 0xE8; // call
		for (std::size_t i = 0; i < 4; ++i)
			*reinterpret_cast<u8*>(addr+5+i) = hookSwapWindowPatch[i];
	});
}

static int PollEvent(SDL_Event* event)
{
	int ret = pollEventHook->GetOriginalFunction<SDL_PollEvent_t>()(event);
	Shortcuts::PollEvent(event);

	HandleSDLEvent(event);

	return ret;
}

void SDL2::HookPollEvent()
{
	// 53                    - push rbx
	// 83 EC 18              - sub esp,18
	// E8 BB8EFDFF           - call libSDL2-2.0.so.0+A6F6 <- must be patched (same as swapwindow)
	// 81 C3 6D370A00        - add ebx,000A376D
	// C7 44 24 04 00000000  - mov [rsp+04],00000000
	pollEventHook = new Tramp(GetSymbolAddress<std::uintptr_t>("libSDL2-2.0.so.0", "SDL_PollEvent"), reinterpret_cast<std::uintptr_t>(&PollEvent), 9,
	[](std::uintptr_t addr, std::uintptr_t gateway)
	{
		*reinterpret_cast<u8*>(gateway+4) = 0xBB; // mov into ebx
		for (std::size_t i = 0; i < 4; ++i)
		{
			hookPollEventPatch[i] = *reinterpret_cast<u8*>(gateway+5+i); // Store for later
			*reinterpret_cast<u8*>(gateway+5+i) = ((addr+9) >> 8*i) & 0xFF;
		}
	});
}

void SDL2::UnhookPollEvent()
{
	pollEventHook->Release([](std::uintptr_t addr, std::uintptr_t gateway)
	{
		*reinterpret_cast<u8*>(addr+4) = 0xE8; // call
		for (std::size_t i = 0; i < 4; ++i)
			*reinterpret_cast<u8*>(addr+5+i) = hookPollEventPatch[i];
	});
}

#endif // HOOKS_SDL2_HPP
