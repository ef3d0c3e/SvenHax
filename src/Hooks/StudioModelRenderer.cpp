#include "Hooks.hpp"

std::vector<Entity*> StudioModelRenderer::monsterList;

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
	monsterList.push_back(monster);
	return studioRendererVMT->GetOriginalMethod<i32(*)(CStudioModelRenderer*, i32, Entity*)>(5)(thisptr, flags, monster);
}
