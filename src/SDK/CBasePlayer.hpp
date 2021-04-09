#ifndef SDK_CBASEPLAYER_HPP
#define SDK_CBASEPLAYER_HPP

#include "../Engine/Entity.hpp"
#include "../Engine/Engine.hpp"
#include "../Util/Util.hpp"

class CBasePlayer
{
public:
	virtual void* MyMonsterPointer()
	{
		return getvfunc<void*(*)(CBasePlayer*)>(this, 28)(this);
	}

	virtual bool IsAlive()
	{
		return getvfunc<bool(*)(CBasePlayer*)>(this, 47)(this);
	}

	virtual Vec3 Center()
	{
		return getvfunc<Vec3(*)(CBasePlayer*)>(this, 71)(this);
	}

	virtual Vec3 EyePosition()
	{
		return getvfunc<Vec3(*)(CBasePlayer*)>(this, 72)(this);
	}

	virtual Vec3 EarPosition()
	{
		return getvfunc<Vec3(*)(CBasePlayer*)>(this, 73)(this);
	}

	virtual bool IsConnected()
	{
		return getvfunc<bool(*)(CBasePlayer*)>(this, 218)(this);
	}

};

static constexpr std::size_t MaxPlayers = 32;
extern CBasePlayer* gPlayerList;

#endif // SDK_CBASEPLAYER_HPP
