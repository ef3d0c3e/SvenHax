#include "Hooks.hpp"
#include "../Util/Hooker.hpp"

#include "../Misc.hpp"
#include "../Hacks/AntiAim.hpp"
#include "../Hacks/BHop.hpp"
#include "../Hacks/AutoStrafer.hpp"

static u8 hookCreateMovePatch[4] = { 0, 0, 0, 0 };
Tramp* createMoveHook = nullptr;

bool CreateMove::sendPacket = true;
QAngle CreateMove::lastViewAngles(0, 0, 0);
void Hooks::CreateMove(f32 frameTime, UserCmd* cmd, i32 active)
{
	createMoveHook->GetOriginalFunction<ClientDLLFuncs::CreateMoveFn>()(frameTime, cmd, active);

	if (!cmd)
		return;

	Misc::CreateMove(cmd);
	BHop::CreateMove(cmd);
	AutoStrafer::CreateMove(cmd);
	AntiAim::CreateMove(cmd);

	if (CreateMove::sendPacket)
		CreateMove::lastViewAngles = cmd->viewAngles;
}

void CreateMove::HookCreateMove()
{

	// E8 97BBF2FF           - call hw.__x86.get_pc_thunk.ax <- moves the address of the next instruction into eax
	// 05 EBE01B00           - add eax,001BE0EB
	// 8B 80 7880D902        - mov eax,[rax+02D98078]
	// 85 C0                 - test eax,eax
	createMoveHook = new Tramp(reinterpret_cast<std::uintptr_t>(gClientDllFuncs->CreateMove), reinterpret_cast<std::uintptr_t>(&Hooks::CreateMove), 5,
	[](std::uintptr_t addr, std::uintptr_t gateway)
	{
		*reinterpret_cast<u8*>(gateway) = 0xB8; // mov into eax
		for (std::size_t i = 0; i < 4; ++i)
		{
			hookCreateMovePatch[i] = *reinterpret_cast<u8*>(gateway+1+i); // Store for later
			*reinterpret_cast<u8*>(gateway+1+i) = ((addr+5) >> 8*i) & 0xFF;
		}
	});
}

void CreateMove::UnhookCreateMove()
{
	createMoveHook->Release([](std::uintptr_t addr, std::uintptr_t gateway)
	{
		*reinterpret_cast<u8*>(addr) = 0xE8; // call
		for (std::size_t i = 0; i < 4; ++i)
			*reinterpret_cast<u8*>(addr+1+i) = hookCreateMovePatch[i];
	});
}
