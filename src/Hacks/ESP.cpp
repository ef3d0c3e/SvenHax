#include "ESP.hpp"
#include "../Engine/Primitives/Matrix.hpp"
#include "../Engine/Primitives/MStudioModel.hpp"
#include "../Engine/TempEnt.hpp"
#include "../Util/Draw.hpp"
#include "../Hooks/Hooks.hpp"
#include "Info.hpp"

namespace Settings::ESP
{
	bool enabled = true;
	i32 maxDormantTime = 500;
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
		const Vec3 min = ent.baseline.mins + ent.origin;
		const Vec3 max = ent.baseline.maxs + ent.origin;

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
//	static const std::array<std::vector<i32>, Settings::ESP::EnemyEntity::size> bones{};
//	const Model* model = gStudioApi->GetModelByIndex(ent.modelIndex);
//	if (!model)
//		return;
}

static void DrawEnemy(const Info::EntInfo& einfo)
{
	//DrawEnemySkeleton(ent, index, ImColor(0.f, 0.f, 1.f, 1.f));
	if (einfo.ent.model)
	{
		float dormant = (einfo.dormant)
			? std::max(0.f,
					std::chrono::duration<float, std::milli>(SDL2::frameBegin - einfo.lastUpdated).count()
					/ Settings::ESP::maxDormantTime)
			: 1.f;
		DrawBox(einfo.ent, ImColor(0.5f, 1.f, 0.5f, dormant), Settings::ESP::BOX_2D);
		ImVec2 pos;
		if (ESP::WorldToScreen(einfo.ent.origin, pos))
			return;

		pos.y+=22;
		Draw::ImText(pos, ImColor(1.f, 0.f, 1.f, dormant), einfo.GetClassName().c_str());
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

	for (const auto& [id, einfo] : Info::entityInfo)
	{
		if (einfo.GetClassFlags() & Info::ClassFlags::DEAD_BODY)
			continue;
		if (einfo.GetClassFlags() & Info::ClassFlags::ENEMY)
			DrawEnemy(einfo);
	}
}


