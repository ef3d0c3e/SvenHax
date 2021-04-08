#ifndef SDK_CBASEPLAYER_HPP
#define SDK_CBASEPLAYER_HPP

#include "../Engine/Entity.hpp"
#include "../Engine/Engine.hpp"

class CBasePlayer
{
	i32 m_idx;
	Entity* m_ent;
public:
	CBasePlayer() {}

	void UpdatePlayer(i32 index)
	{
		m_idx = index;
		m_ent = gEngineFuncs->GetEntityByIndex(index);
	}

	Vec3 GetOrigin() const
	{
		return m_ent->origin;
	}

	QAngle GetAngles() const
	{
		return m_ent->angles;
	}
};

static constexpr std::size_t MaxPlayers = 32;
extern CBasePlayer gPlayerList[MaxPlayers];

#endif // SDK_CBASEPLAYER_HPP
