#include "Hooks.hpp"
#include "../Hacks/Chams.hpp"

i32 StudioModelRenderer::StudioDrawModel(CStudioModelRenderer* thisptr, i32 flags)
{
	return studioRendererVMT->GetOriginalMethod<i32(*)(CStudioModelRenderer*, i32)>(3)(thisptr, flags);
}

i32 StudioModelRenderer::StudioDrawPlayer(CStudioModelRenderer* thisptr, i32 flags, EntityState* player)
{
	return studioRendererVMT->GetOriginalMethod<i32(*)(CStudioModelRenderer*, i32, EntityState*)>(4)(thisptr, flags, player);
}

i32 StudioModelRenderer::StudioDrawMonster(CStudioModelRenderer* thisptr, i32 flags, Entity* monster)
{
	return studioRendererVMT->GetOriginalMethod<i32(*)(CStudioModelRenderer*, i32, Entity*)>(5)(thisptr, flags, monster);
}

void StudioModelRenderer::StudioRenderModel(CStudioModelRenderer* thisptr)
{
	bool handled = false;
	handled = Chams::StudioRenderModel();

	if (!handled)
		studioRendererVMT->GetOriginalMethod<void(*)(CStudioModelRenderer*)>(20)(thisptr);
}
