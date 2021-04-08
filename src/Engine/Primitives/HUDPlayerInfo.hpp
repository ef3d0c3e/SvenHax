#ifndef ENGINE_PRIMITIVES_HUDPLAYERINFO_HPP
#define ENGINE_PRIMITIVES_HUDPLAYERINFO_HPP

#include "Defs.hpp"

struct HUDPlayerInfo
{
	char *name;
	i16 ping;
	u8 thisPlayer;  // TRUE if this is the calling player

	u8 spectator;
	u8 packetLoss;

	char *model;
	i16 topcolor;
	i16 bottomColor;

	u64 steamID;
};

#endif // ENGINE_PRIMITIVES_HUDPLAYERINFO_HPP
