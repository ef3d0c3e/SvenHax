#include "Hooks.hpp"
#include "../Engine/ClientDLL.hpp"
#include "../Interface.hpp"
#include "../Util/Hooker.hpp"
#include "../Hacks/AntiAim.hpp"

// {{{ Redraw
Tramp* redrawHook = nullptr;
static u8 redrawHookPatch[4] = { 0, 0, 0, 0 };

i32 RedrawHook(f32 time, i32 intermission)
{
	i32 ret = redrawHook->GetOriginalFunction<i32(*)(float, i32)>()(time, intermission);

	return ret;
}

void ClientDLL::HookRedraw()
{

	// ClientDLL_HudRedraw(int) <+0>  53                 push   %ebx
	// ClientDLL_HudRedraw(int) <+1>  e8 5a 8f f2 ff     call   0xefa4f1e0 <__x86.get_pc_thunk.bx> <-- needs to be patched
	// ClientDLL_HudRedraw(int) <+6>  81 c3 7a dd 1b 00  add    $0x1bdd7a,%ebx
	// ClientDLL_HudRedraw(int) <+12> 83 ec 28           sub    $0x28,%esp

	redrawHook = new Tramp(reinterpret_cast<std::uintptr_t>(gClientDllFuncs->HudRedraw), reinterpret_cast<std::uintptr_t>(&RedrawHook), 6,
	[](std::uintptr_t addr, std::uintptr_t gateway)
	{
		*reinterpret_cast<u8*>(gateway+1) = 0xBB; // mov into ebx
		for (std::size_t i = 0; i < 4; ++i)
		{
			redrawHookPatch[i] = *reinterpret_cast<u8*>(gateway+2+i); // Store for later
			*reinterpret_cast<u8*>(gateway+2+i) = ((addr+6) >> 8*i) & 0xFF;
		}
	});
}

void ClientDLL::UnhookRedraw()
{
	redrawHook->Release([](std::uintptr_t addr, std::uintptr_t gateway)
	{
		*reinterpret_cast<u8*>(addr+1) = 0xE8; // call
		for (std::size_t i = 0; i < 4; ++i)
			*reinterpret_cast<u8*>(addr+2+i) = redrawHookPatch[i];
	});
}
// }}}
