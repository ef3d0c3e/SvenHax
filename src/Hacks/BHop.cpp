#include "BHop.hpp"

namespace Settings::BHop
{
	bool enabled = true;
}

void BHop::CreateMove(UserCmd* cmd)
{
	if (!Settings::BHop::enabled)
		return;

	if (!gameEngine->IsInGame())
		return;

	if (gPMove->moveType == MoveType::FLY || gPMove->moveType == MoveType::NOCLIP)
		return;

	static bool jumpedLastTick = false;
	static bool avoid = false;

	if (!jumpedLastTick && avoid)
	{
		avoid = false;
		cmd->buttons |= IN_JUMP;
	}
	else if (cmd->buttons & IN_JUMP)
	{
		if (gPMove->flags & MoveFlags::ONGROUND)
		{
			jumpedLastTick = true;
			avoid = true;
		}
		else
		{
			cmd->buttons &= ~IN_JUMP;
			jumpedLastTick = false;
		}
	}
	else
	{
		jumpedLastTick = false;
		avoid = false;
	}
}
