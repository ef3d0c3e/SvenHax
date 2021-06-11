#include "ESP.hpp"
#include "../Engine/Primitives/Matrix.hpp"
#include "../Engine/TempEnt.hpp"
#include "../Util/Draw.hpp"

namespace Settings::ESP
{
	bool enabled = true;
}

Mat3x3 viewMat;

bool ESP::WorldToScreen(const Vec3& point, ImVec2& screen)
{
	bool v = gEngineFuncs->pTriAPI->WorldToScreen((float*)&point, (float*)&screen);
	screen.x =   screen.x*gEngineFuncs->GetWindowCenterX() + gEngineFuncs->GetWindowCenterX();
	screen.y = - screen.y*gEngineFuncs->GetWindowCenterY() + gEngineFuncs->GetWindowCenterY();

	return v;
}

static bool GetBox( const EntVars& ent, int& x, int& y, int& w, int& h )
{
	// Variables
	Vec3 vOrigin, min, max;
	ImVec2 flb, brt, blb, frt, frb, brb, blt, flt; // think of these as Front-Left-Bottom/Front-Left-Top... Etc.
	f32 left, top, right, bottom;

	// Get the locations
	vOrigin = ent.origin;
	min = ent.mins + vOrigin;
	max = ent.maxs + vOrigin;

	// Points of a 3d bounding box
	Vec3 points[] = { Vec3( min.x, min.y, min.z ),
						Vec3( min.x, max.y, min.z ),
						Vec3( max.x, max.y, min.z ),
						Vec3( max.x, min.y, min.z ),
						Vec3( max.x, max.y, max.z ),
						Vec3( min.x, max.y, max.z ),
						Vec3( min.x, min.y, max.z ),
						Vec3( max.x, min.y, max.z ) };

	// Get screen positions
	if ( ESP::WorldToScreen( points[3], flb ) || ESP::WorldToScreen( points[5], brt )
		 || ESP::WorldToScreen( points[0], blb ) || ESP::WorldToScreen( points[4], frt )
		 || ESP::WorldToScreen( points[2], frb ) || ESP::WorldToScreen( points[1], brb )
		 || ESP::WorldToScreen( points[6], blt ) || ESP::WorldToScreen( points[7], flt ) )
		return false;

	// Put them in an array (maybe start them off in one later for speed?)
	ImVec2 arr[] = { flb, brt, blb, frt, frb, brb, blt, flt };

	// Init this shit
	left = flb.x;
	top = flb.y;
	right = flb.x;
	bottom = flb.y;

	// Find the bounding corners for our box
	for ( int i = 1; i < 8; i++ ) {
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
	x = ( int ) left;
	y = ( int ) top;
	w = ( int ) ( right - left );
	h = ( int ) ( bottom - top );

	return true;
}

static void DrawMonster(const EntVars& ent)
{
	Vec3 vOrigin = ent.origin;
	Vec3 min = ent.mins + vOrigin;
	Vec3 max = ent.maxs + vOrigin;

	Vec3 points[] = { Vec3( min.x, min.y, min.z ),
						Vec3( min.x, max.y, min.z ),
						Vec3( max.x, max.y, min.z ),
						Vec3( max.x, min.y, min.z ),
						Vec3( min.x, min.y, max.z ),
						Vec3( min.x, max.y, max.z ),
						Vec3( max.x, max.y, max.z ),
						Vec3( max.x, min.y, max.z ) };

	i32 edges[12][2] = {
			{ 0, 1 },
			{ 1, 2 },
			{ 2, 3 },
			{ 3, 0 },
			{ 4, 5 },
			{ 5, 6 },
			{ 6, 7 },
			{ 7, 4 },
			{ 0, 4 },
			{ 1, 5 },
			{ 2, 6 },
			{ 3, 7 },
	};

	for ( const auto edge : edges ) {
		ImVec2 p1, p2;
		if ( ESP::WorldToScreen( points[edge[0]], p1 ) || ESP::WorldToScreen( points[edge[1]], p2 ) )
			return;
		Draw::ImLine( p1, p2, ImColor(0.5f, 1.f, 0.f, 1.f) );
	}

	/*
	i32 x, y, w, h;
	if (!GetBox(ent, x, y, w, h))
	
	Draw::ImRect(x, y, x+w, y+h, ImColor(1.f, 0.f, 0.f, 1.f));
	*/
}

void DrawWeapon(const EntVars& ent)
{

}

void DrawAmmo(const EntVars& ent)
{

}

void DrawItem(const EntVars& ent)
{

}

void DrawPlayer(const EntVars& ent)
{

}

void ESP::Paint()
{
	if (!Settings::ESP::enabled)
		return;

	if (!gameEngine->IsInGame())
		return;

	for (std::size_t i = 0; i < 2048; ++i)
	{
		ImVec2 pos;
		const auto ptr = gFuncs->EntityOfEntIndex(i);
		if (ptr == nullptr)
			continue;

		const auto className = gFuncs->StringFromIndex(ptr->vars.className);
		if (strncmp("monster_", className, 7) == 0)
			DrawMonster(ptr->vars);
		else if (strncmp("weapon_", className, 6) == 0)
			DrawWeapon(ptr->vars);
		else if (strncmp("ammo_", className, 4) == 0)
			DrawAmmo(ptr->vars);
		else if (strncmp("item_", className, 4) == 0)
			DrawItem(ptr->vars);
		else if (strncmp("player", className, 6) == 0)
			DrawPlayer(ptr->vars);
	}
}


