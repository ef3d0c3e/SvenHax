#ifndef ENGINE_PRIMITIVES_SCREENFADE_HPP
#define ENGINE_PRIMITIVES_SCREENFADE_HPP

#include "Defs.hpp"
#include "Color.hpp"

struct ScreenFade
{
	f32 fadeSpeed; // How fast to fade (tics / second) (+ fade in, - fade out)
	f32 fadeEnd; // When the fading hits maximum
	f32 fadeTotalEnd; // Total End Time of the fade (used for FFADE_OUT)
	f32 fadeReset; // When to reset to not fading (for fadeout and hold)
	Color32 color;
	i32 fadeFlags; // Fading flags
};

#endif // ENGINE_PRIMITIVES_SCREENFADE_HPP
