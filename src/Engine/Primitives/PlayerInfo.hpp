#ifndef ENGINE_PRIMITIVES_PLAYERINFO_HPP
#define ENGINE_PRIMITIVES_PLAYERINFO_HPP

#include "Customization.hpp"
#include "Vector.hpp"

struct PlayerInfo
{
	// User id on server
	i32 userid;
	
	// User info string
	static constexpr inline std::size_t InfoStringLen = 256;
	std::array<char, InfoStringLen> userInfo;
	
	// Name
	static constexpr inline std::size_t ScoreboardNameLen = 32;
	std::array<char, ScoreboardNameLen> name;
	
	// Spectator or not, unused
	i32 spectator;
	
	i32 ping;
	i32 packetLoss;
	
	// skin information
	std::array<char, Resource::QPathLen> model;
	i32 topColor;
	i32 bottomColor;
	
	// last frame rendered
	i32 renderFrame;
	
	// Gait frame estimation
	i32 gaitSequence;
	f32 gaitFrame;
	f32 gaitYaw;
	Vec3 prevGaitOrigin;
	
	Customization customData;
};

#endif // ENGINE_PRIMITIVES_PLAYERINFO_HPP
