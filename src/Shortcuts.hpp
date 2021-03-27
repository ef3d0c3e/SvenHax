#ifndef SHORTCUTS_HPP
#define SHORTCUTS_HPP

#include "SDK/SDK.hpp"
#include <SDL2/SDL.h>

namespace Shortcuts
{
	void PollEvent(SDL_Event* event);
	void SetKeyCodeState(KeyCode code, bool bPressed);
}

#endif // SHORTCUTS_HPP
