#ifndef ENGINE_PRIMITIVES_MOUTH_HPP
#define ENGINE_PRIMITIVES_MOUTH_HPP

#include "Defs.hpp"

struct Mouth
{
	u8 mouthopen; // 0 = mouth closed, 255 = mouth agape
	u8 sndcount; // counter for running average
	i32 sndavg; // running average
};

#endif // ENGINE_PRIMITIVES_MOUTH_HPP
