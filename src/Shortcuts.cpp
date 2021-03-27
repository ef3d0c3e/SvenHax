#include "Shortcuts.hpp"

#include "UI/UI.hpp"

void Shortcuts::PollEvent(SDL_Event* event)
{
	// Menu
	if (((event->key.keysym.sym == SDLK_INSERT && event->type == SDL_KEYDOWN))
	|| ((event->key.keysym.mod & KMOD_LALT) && event->key.keysym.sym == SDLK_i && event->type == SDL_KEYDOWN)){
		UI::SetVisible(!UI::isVisible);
    }
}

void Shortcuts::SetKeyCodeState(KeyCode code, bool bPressed)
{
}
