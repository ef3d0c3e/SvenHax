#ifndef HOOKS_SDL2_HPP
#define HOOKS_SDL2_HPP

#include "../ImGui/imgui.h"
#include "../ImGui/examples/imgui_impl_opengles3.h"
#include "../ImGui/examples/libs/gl3w/GL/gl3w.h"
#include "../ImGui/imgui_internal.h" // for 	ImGui::GetCurrentContext()->Font->DisplayOffset
#include "../Interface.hpp"
#include "../Settings.hpp"
#include "../Shortcuts.hpp"
#include "../UI/UI.hpp"
#include "../UI/FontPlex.hpp"
#include "../UI/FontPlexMono.hpp"
#include "../UI/FontTitle.hpp"
#include "../Util/Hooker.hpp"
#include "../Util/Util.hpp"
#include "Hooks.hpp"
#include <SDL2/SDL.h>

#include <fmt/format.h>

int SDL2::windowWidth = 0;
int SDL2::windowHeight = 0;
ImFont* UI::plex = nullptr;
ImFont* UI::plex_mono = nullptr;
ImFont* UI::title_font = nullptr;


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

		// Fonts
		//io.FontGlobalScale = 0.5f; // We perform expensive upscaling to get some neat looking result, though there should be a cheaper way
		ImFontConfig config;
		config.OversampleH = 4;
		config.OversampleV = 4;
		config.PixelSnapH = false;
		config.SizePixels = 40;
		UI::plex = io.Fonts->AddFontFromMemoryCompressedBase85TTF(plex_compressed_data_base85, 20.f, &config);
		UI::plex_mono = io.Fonts->AddFontFromMemoryCompressedBase85TTF(plex_mono_compressed_data_base85, 20.f, &config);
		UI::title_font = io.Fonts->AddFontFromMemoryCompressedBase85TTF(title_compressed_data_base85, 40.f, &config);

		
		// Colors
		ImVec4* colors = ImGui::GetStyle().Colors;
		colors[ImGuiCol_Text]                  = ImVec4(0.75f, 0.75f, 0.75f, 1.00f);
		colors[ImGuiCol_TextDisabled]          = ImVec4(0.35f, 0.35f, 0.35f, 1.00f);
		colors[ImGuiCol_WindowBg]              = ImVec4(0.00f, 0.00f, 0.00f, 0.94f);
		colors[ImGuiCol_ChildBg]               = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		colors[ImGuiCol_PopupBg]               = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
		colors[ImGuiCol_Border]                = ImVec4(1.00f, 0.50f, 0.50f, 0.50f);
		colors[ImGuiCol_BorderShadow]          = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		colors[ImGuiCol_FrameBg]               = ImVec4(0.00f, 0.00f, 0.00f, 0.54f);
		colors[ImGuiCol_FrameBgHovered]        = ImVec4(0.37f, 0.14f, 0.14f, 0.67f);
		colors[ImGuiCol_FrameBgActive]         = ImVec4(0.39f, 0.20f, 0.20f, 0.67f);
		colors[ImGuiCol_TitleBg]               = ImVec4(0.04f, 0.04f, 0.04f, 1.00f);
		colors[ImGuiCol_TitleBgActive]         = ImVec4(0.16f, 0.16f, 0.16f, 1.00f);
		colors[ImGuiCol_TitleBgCollapsed]      = ImVec4(0.48f, 0.16f, 0.16f, 1.00f);
		colors[ImGuiCol_MenuBarBg]             = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
		colors[ImGuiCol_ScrollbarBg]           = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		colors[ImGuiCol_ScrollbarGrab]         = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
		colors[ImGuiCol_ScrollbarGrabHovered]  = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
		colors[ImGuiCol_ScrollbarGrabActive]   = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
		colors[ImGuiCol_CheckMark]             = ImVec4(0.96f, 0.96f, 0.96f, 1.00f);
		colors[ImGuiCol_SliderGrab]            = ImVec4(1.00f, 0.19f, 0.19f, 0.40f);
		colors[ImGuiCol_SliderGrabActive]      = ImVec4(0.89f, 0.00f, 0.19f, 1.00f);
		colors[ImGuiCol_Button]                = ImVec4(0.19f, 0.19f, 0.39f, 1.00f);
		colors[ImGuiCol_ButtonHovered]         = ImVec4(0.80f, 0.17f, 0.00f, 1.00f);
		colors[ImGuiCol_ButtonActive]          = ImVec4(0.89f, 0.00f, 0.19f, 1.00f);
		colors[ImGuiCol_Header]                = ImVec4(0.33f, 0.35f, 0.36f, 0.53f);
		colors[ImGuiCol_HeaderHovered]         = ImVec4(0.76f, 0.28f, 0.44f, 0.67f);
		colors[ImGuiCol_HeaderActive]          = ImVec4(0.47f, 0.47f, 0.47f, 0.67f);
		colors[ImGuiCol_Separator]             = ImVec4(0.32f, 0.32f, 0.32f, 1.00f);
		colors[ImGuiCol_SeparatorHovered]      = ImVec4(0.32f, 0.32f, 0.32f, 1.00f);
		colors[ImGuiCol_SeparatorActive]       = ImVec4(0.32f, 0.32f, 0.32f, 1.00f);
		colors[ImGuiCol_ResizeGrip]            = ImVec4(1.00f, 1.00f, 1.00f, 0.85f);
		colors[ImGuiCol_ResizeGripHovered]     = ImVec4(1.00f, 1.00f, 1.00f, 0.60f);
		colors[ImGuiCol_ResizeGripActive]      = ImVec4(1.00f, 1.00f, 1.00f, 0.90f);
		colors[ImGuiCol_PlotLines]             = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
		colors[ImGuiCol_PlotLinesHovered]      = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
		colors[ImGuiCol_PlotHistogram]         = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
		colors[ImGuiCol_PlotHistogramHovered]  = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
		colors[ImGuiCol_TextSelectedBg]        = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
		colors[ImGuiCol_DragDropTarget]        = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
		colors[ImGuiCol_NavHighlight]          = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
		colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
		colors[ImGuiCol_NavWindowingDimBg]     = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
		colors[ImGuiCol_ModalWindowDimBg]      = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);

		UI::UpdateColors();

	
		ImGuiStyle* style = &ImGui::GetStyle();
		style->WindowPadding = ImVec2(0, 0);
		style->FramePadding = ImVec2(4, 4);
		style->ItemSpacing = ImVec2(0, 0);
		style->ItemInnerSpacing = ImVec2(4, 4);
		style->IndentSpacing = 10;
		style->ScrollbarSize = 12;
		style->GrabMinSize = 4;
		
		style->WindowRounding = 0;
		style->ChildRounding = 4;
		style->FrameRounding = 4;
		style->PopupRounding = 0;
		style->ScrollbarRounding = 4;
		style->GrabRounding = 0;
		
		style->WindowBorderSize = 0;
		style->ChildBorderSize = 0;
		style->PopupBorderSize = 0;
		style->FrameBorderSize = 0;

		ImGui_ImplOpenGL3_Init("#version 100");

		bFirst = false;
	}

	ImGuiIO& io = ImGui::GetIO();
	// Setup display size (every frame to accommodate for window resizing)
	int w, h;
	SDL_GetWindowSize(window, &w, &h);
	SDL2::windowWidth = w;
	SDL2::windowHeight = h;
	io.DisplaySize = ImVec2((float)w, (float)h);
	io.DisplayFramebufferScale = ImVec2( 1, 1 );

	ImGui_ImplOpenGL3_NewFrame();

	static double lastTime = 0.0f;
	Uint32 time = SDL_GetTicks();
	double currentTime = time / 1000.0;
	io.DeltaTime = lastTime > 0.0 ? (float)(currentTime - lastTime) : (float)(1.0f / 60.0f);

	io.MouseDrawCursor = UI::isVisible && gameEngine->IsInGame();
	io.WantCaptureMouse = UI::isVisible;
	io.WantCaptureKeyboard = UI::isVisible;

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

	UI::Draw();

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
	Shortcuts::PollEvent(event);

	HandleSDLEvent(event);

	return pollEventHook->GetOriginalFunction<SDL_PollEvent_t>()(event);
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
