#include "ESP.hpp"
#include "../Engine/Primitives/Matrix.hpp"
#include "../Engine/Primitives/MStudioModel.hpp"
#include "../Engine/TempEnt.hpp"
#include "../Util/Draw.hpp"
#include "../Hooks/Hooks.hpp"

namespace Settings::ESP
{
	bool enabled = true;
}

bool ESP::WorldToScreen(const Vec3& point, ImVec2& screen)
{
	bool v = gEngineFuncs->pTriAPI->WorldToScreen((float*)&point, (float*)&screen);
	screen.x =   screen.x*gEngineFuncs->GetWindowCenterX() + gEngineFuncs->GetWindowCenterX();
	screen.y = - screen.y*gEngineFuncs->GetWindowCenterY() + gEngineFuncs->GetWindowCenterY();

	return v;
}

static void DrawBox(const Entity& ent, const ImColor& color, Settings::ESP::BoxType type)
{
	// {{{ Get 2D Box
	auto get2DBox = [](const Entity& ent, Vec2i& pos, Vec2i& size)
	{
		// Variables
		ImVec2 flb, brt, blb, frt, frb, brb, blt, flt; // think of these as Front-Left-Bottom/Front-Left-Top... Etc.
		f32 left, top, right, bottom;

		// Get the locations
		const Vec3 min = ent.model->mins + ent.origin;
		const Vec3 max = ent.model->maxs + ent.origin;

		// Points of a 3d bounding box
		std::array<Vec3, 8> points = {
			Vec3(min.x, min.y, min.z),
			Vec3(min.x, max.y, min.z),
			Vec3(max.x, max.y, min.z),
			Vec3(max.x, min.y, min.z),
			Vec3(max.x, max.y, max.z),
			Vec3(min.x, max.y, max.z),
			Vec3(min.x, min.y, max.z),
			Vec3(max.x, min.y, max.z)
		};

		// Get screen positions
		if (ESP::WorldToScreen(points[3], flb) || ESP::WorldToScreen(points[5], brt) ||
			ESP::WorldToScreen(points[0], blb) || ESP::WorldToScreen(points[4], frt) ||
			ESP::WorldToScreen(points[2], frb) || ESP::WorldToScreen(points[1], brb) ||
			ESP::WorldToScreen(points[6], blt) || ESP::WorldToScreen(points[7], flt))
			return false;

		// Put them in an array (maybe start them off in one later for speed?)
		std::array<ImVec2, 8> arr{flb, brt, blb, frt, frb, brb, blt, flt};

		// Init this shit
		left = flb.x;
		top = flb.y;
		right = flb.x;
		bottom = flb.y;

		// Find the bounding corners for our box
		for (std::size_t i = 1; i < 8; ++i)
		{
			if ( left > arr[i].x )
				left = arr[i].x;
			if ( bottom < arr[i].y )
				bottom = arr[i].y;
			if ( right < arr[i].x )
				right = arr[i].x;
			if ( top > arr[i].y )
				top = arr[i].y;
		}

		// Width / height
		pos.x = left;
		pos.y = top;
		size.x = right-left;
		size.y = bottom-top;

		return true;
	};
	// }}}
	
	if (type == Settings::ESP::BOX_2D)
	{
		Vec2i pos(0,0), size(0,0);
		if (!get2DBox(ent, pos, size))
			return;
		Draw::ImRect(pos.x, pos.y, pos.x+size.x, pos.y+size.y, color);
	}
	else if (type == Settings::ESP::BOX_2D_FLAT)
	{
		Vec2i pos(0,0), size(0,0);
		if (!get2DBox(ent, pos, size))
			return;
		Draw::ImRectFilled(pos.x+1, pos.y+1, pos.x+size.x-1, pos.y+size.y-1, ImColor(color.Value.x, color.Value.y, color.Value.z, color.Value.w*0.25f));
		Draw::ImRect(pos.x, pos.y, pos.x+size.x, pos.y+size.y, color);
	}
	else if (type == Settings::ESP::BOX_3D)
	{
		const Vec3 min = ent.baseline.mins + ent.origin;
		const Vec3 max = ent.baseline.maxs + ent.origin;

		std::array<Vec3, 8> points
		{
			Vec3(min.x, min.y, min.z),
			Vec3(min.x, max.y, min.z),
			Vec3(max.x, max.y, min.z),
			Vec3(max.x, min.y, min.z),
			Vec3(min.x, min.y, max.z),
			Vec3(min.x, max.y, max.z),
			Vec3(max.x, max.y, max.z),
			Vec3(max.x, min.y, max.z)
		};
		static constexpr std::array<std::array<i32, 2>, 12> edges
		{
			std::array<i32, 2>{0, 1}, std::array<i32, 2>{1, 2}, std::array<i32, 2>{2, 3}, std::array<i32, 2>{3, 0},
			std::array<i32, 2>{4, 5}, std::array<i32, 2>{5, 6}, std::array<i32, 2>{6, 7}, std::array<i32, 2>{7, 4},
			std::array<i32, 2>{0, 4}, std::array<i32, 2>{1, 5}, std::array<i32, 2>{2, 6}, std::array<i32, 2>{3, 7}
		};

		for (const auto edge : edges)
		{
			ImVec2 p1, p2;
			if (ESP::WorldToScreen(points[edge[0]], p1) ||
				ESP::WorldToScreen(points[edge[1]], p2))
				return;
			Draw::ImLine(p1, p2, ImColor(0.5f, 1.f, 0.f, 1.f));
		}
	}
}

static void DrawEnemySkeleton(const EntVars& ent, std::size_t index, const ImColor& color)
{
	static const std::array<std::vector<i32>, Settings::ESP::EnemyEntity::size> bones{};
	const Model* model = gStudioApi->GetModelByIndex(ent.modelIndex);
	if (!model)
		return;
}

static void DrawEnemy(Entity& ent, std::size_t index)
{
	//DrawEnemySkeleton(ent, index, ImColor(0.f, 0.f, 1.f, 1.f));
	if (ent.model)
	{
		DrawBox(ent, ImColor(0.5f, 1.f, 0.5f, 1.f), Settings::ESP::BOX_3D);
		ImVec2 pos;
		if (ESP::WorldToScreen(ent.origin, pos))
			return;
		pos.y+=22;
		Draw::ImText(pos, ImColor(1.f, 0.f, 1.f, 1.f), fmt::format("{0:p}", (const void*)&ent).c_str());
		pos.y+=18;
		Draw::ImText(pos, ImColor(1.f, 0.f, 1.f, 1.f), fmt::format("{0:p}", (const void*)ent.model).c_str());
	}
}

static void DrawPlayer(const EntVars& ent)
{
	//DrawBox(ent, ImColor(1.0f, 0.f, 0.8f, 1.f), Settings::ESP::BOX_2D_FLAT);
}

void ESP::Paint()
{
	if (!Settings::ESP::enabled)
		return;

	if (!gameEngine->IsInGame())
		return;

	/*
	for (const Entity* ent : StudioModelRenderer::monsterList)
	{
		ImVec2 pos;
		if (ESP::WorldToScreen(ent->origin, pos))
			continue;
		std::cout << (const void*)ent << std::endl;
		DrawBox(*ent, ImColor(0.5f, 1.f, 0.5f, 1.f), Settings::ESP::BOX_3D);
		Draw::ImText(pos, ImColor(0.f, 0.f, 1.f, 1.f), std::to_string(ent->index).c_str());
		pos.y+=18;
		Draw::ImText(pos, ImColor(1.f, 0.f, 1.f, 1.f), fmt::format("{0:p}", (const void*)ent).c_str());
		pos.y+=18;
		Draw::ImText(pos, ImColor(1.f, 0.f, 1.f, 1.f), fmt::format("{0:p}", (const void*)ent->model).c_str());
	}
	*/

	const Entity* localplayer = gEngineFuncs->GetLocalPlayer();
	for (std::size_t i = 2; i < 2048; ++i) // 0 = world
	{
		Entity* ent = gEngineFuncs->GetEntityByIndex(i);
		if (!ent || ent == localplayer)
			continue;
		DrawEnemy(*ent, 0);
	}

	//for (std::size_t i = 0; i < gPMove->numVisEnt; ++i)
	//{
	//	const PhysEnt& ent = gPMove->visEnts[i];

	//	ImVec2 pos;
	//	if (ESP::WorldToScreen(ent.origin, pos))
	//		continue;
	//	Draw::ImText(pos, ImColor(0.f, 0.f, 1.f, 1.f), std::to_string(ent.classnumber).c_str());
	//}

	// Serverside
	//for (std::size_t i = 0; i < EDictEntsNum; ++i)
	//{
	//	std::cout << "maybe nnull" << std::endl;
	//	const EDict* dict = gExportedFuncs->EntityOfEntIndex(i);
	//	if (dict == nullptr)
	//		continue;
	//	std::cout << "not nnull" << std::endl;
 
	//	ImVec2 pos;
	//	if (ESP::WorldToScreen(dict->vars.origin, pos))
	//		continue;
	//	Draw::ImText(pos, ImColor(0.f, 0.f, 1.f, 1.f), std::to_string(dict->vars.className).c_str());
	//	pos.y += 18;
	//	Draw::ImText(pos, ImColor(0.f, 0.f, 1.f, 1.f), fmt::format("{0:p}", (void*)&dict->vars).c_str());
	//}

	//	auto isInList = []<std::size_t N>(const std::array<std::string_view, N>& list, const char* name, std::size_t len)
	//	{
	//		for (std::size_t i = 0 ; i < N; ++i)
	//		{
	//			if (len != list[i].size())
	//				continue;
	//			if (strncmp(list[i].data(), name, list[i].size()) == 0)
	//				return i;
	//		}

	//		return std::size_t(-1);
	//	};

	//	const char* className = gExportedFuncs->StringFromIndex(dict->vars.className);
	//	const std::size_t classNameLen = strlen(className);
	//	if (classNameLen > 8 && strncmp(className, "monster_", 8) == 0) // 'monster_'
	//	{
	//		const auto i = isInList(Settings::ESP::EnemyEntityClass, &className[8], classNameLen-8);
	//		if (i == std::size_t(-1))
	//		{
	//			fmt::print("ESP::Paint() : unknown monster `{}`\n", className);
	//			continue;
	//		}
	//		DrawEnemy(dict->vars, i);
	//	}
	//	else if (classNameLen == 6 && strncmp(className, "player", 6) == 0) // 'player'
	//	{
	//		DrawPlayer(dict->vars);
	//	}


	//	/*
	//	if (strncmp("monster_", className, 8) == 0)
	//		DrawMonster(dict->vars);
	//	else if (strncmp("weapon_", className, 5) == 0)
	//		DrawWeapon(dict->vars);
	//	else if (strncmp("ammo_", className, 5) == 0)
	//		DrawAmmo(dict->vars);
	//	else if (strncmp("item_", className, 5) == 0)
	//		DrawItem(dict->vars);
	//	else if (strncmp("player", className, 6) == 0)
	//		DrawPlayer(dict->vars);
	//	else if (strncmp("env_", className, 4) == 0)
	//		continue;

	//	{
	//		if (WorldToScreen(dict->vars.origin, pos))
	//			continue;
	//		auto&& [x, y] = pos;
	//		Draw::ImText(ImVec2(x, y-20), ImColor(0.f, 1.f, 0.f, 1.f),  className);
	//	}
	//	*/
	//}
}


