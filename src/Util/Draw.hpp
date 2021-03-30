#ifndef UTIL_DRAW_HPP
#define UTIL_DRAW_HPP

#include "../ImGUI/imgui.h"
#include "../SDK/SDK.hpp"
#include "../Engine/Primitives/Vector.hpp"
#include "../Engine/Primitives/Color.hpp"
#include "Cenum.hpp"
#include <deque>

MAKE_CENUMV_Q(ImFontFlags, int,
	ImFontFlags_None, 1 << 1,
	ImFontFlags_Outline, 1 << 2,
	ImFontFlags_Shadow, 1 << 3
);

MAKE_CENUMV_Q(DrawType, int,
	DRAW_LINE, 0,
	DRAW_RECT, 1,
	DRAW_RECT_FILLED, 2,
	DRAW_CIRCLE, 3,
	DRAW_CIRCLE_FILLED, 4,
	DRAW_CIRCLE_3D, 5,
	DRAW_TEXT, 6,
);

//TODO Implement circle drawing for surface compatibility
struct DrawRequest
{
	DrawRequest():
	type(DrawType()),
	x0(int()),
	y0(int()),
	x1(int()),
	y1(int()),
	circleSegments(int()),
	circleRadius(float()),
	thickness(float()),
	color(ImColor()),
	fontflags(ImFontFlags()),
	pos(Vec3()),
	text("") {}

	DrawType type;
	int x0, y0, x1, y1;
	int circleSegments;
	float circleRadius;
	float thickness;
	ImColor color;
	ImFontFlags fontflags;
	Vec3 pos;
	char text[256];
};

namespace Draw
{
extern std::deque<DrawRequest> drawRequests;

// Surface
void Circle(Vec2 position, int segments, float radius, Color32 color);
void OutlinedCircle(int x0, int y0, int segments, int radius, Color32 col);
void FilledCircle(Vec2 position, int segments, float radius, Color32 color);
void Circle3D(const Vec3& position, int segments, float radius, Color32 color);
void FilledRectangle(int x0, int y0, int x1, int y1, Color32 col);
void FilledRectangle(Vec2 start_pos, Vec2 end_pos, Color32 col);
void Rectangle(int x0, int y0, int x1, int y1, Color32 col);
void Rectangle(Vec2 start_pos, Vec2 end_pos, Color32 col);
void Line(int x0, int y0, int x1, int y1, Color32 col);
void Line(Vec2 start_pos, Vec2 end_pos, Color32 col);
void PolyLine(int* px, int* py, int num_points, Color32 col);
void PolyLine(VGuiVertex* vertice, int num_points, Color32 col);
void TexturedPolygon(int n, VGuiVertex* vertice, Color32 col);
void TextW(int x, int y, const wchar_t* text, HFont font, Color32 col);
void TextW(Vec2 pos, const wchar_t* text, HFont font, Color32 col);
void Text(int x, int y, const char* text, HFont font, Color32 col);
void Text(Vec2 pos, const char* text, HFont font, Color32 col);
void GetTextWSize(const wchar_t* text, HFont font, int& wide, int& tall);
Vec2 GetTextWSize(const wchar_t* text, HFont font);
void GetTextSize(const char* text, HFont font, int& wide, int& tall);
Vec2 GetTextSize(const char* text, HFont font);

// ImGui
void ImCircle(ImVec2 point, ImColor color, float radius, int num_segments = 12, float thickness = 1.0f);
void ImCircle3D(Vec3 position, int segments, float radius, ImColor color);
void ImCircleFilled(ImVec2 point, ImColor color, float radius, int num_segments = 12);
void ImImage(const char* const imagePath, ImVec2 a, ImVec2 b);
void ImLine(ImVec2 a, ImVec2 b, ImColor color, float thickness = 1.0f);
void ImRect(ImVec2 a, ImVec2 b, ImColor color, float rounding = 0.0f, int rounding_corners_flags = ~0, float thickness = 1.0f);
void ImRect(int x0, int y0, int x1, int y1, ImColor color, float rounding = 0.0f, int rounding_corners_flags = ~0, float thickness = 1.0f);
void ImRectFilled(int x0, int y0, int x1, int y1, ImColor color, float rounding = 0.0f, int rounding_corners_flags = ~0);
void ImRectFilled(ImVec2 a, ImVec2 b, ImColor color, float rounding = 0.0f, int rounding_corners_flags = ~0);
void ImText(ImVec2 pos, ImColor color, const char* text_begin, const char* text_end = nullptr, float wrap_width = 0.0f, const ImVec4* cpu_fine_clip_rect = nullptr, ImFontFlags flags = ImFontFlags_Outline);

// Functions to Add to DrawRequests
void AddLine(int x0, int y0, int x1, int y1, ImColor color);
void AddRect(int x0, int y0, int x1, int y1, ImColor color);
void AddRectFilled(int x0, int y0, int x1, int y1, ImColor color);
void AddCircle(int x0, int y0, float radius, ImColor color, int segments = 12, float thickness = 1.0f);
void AddCircleFilled(int x0, int y0, float radius, ImColor color, int segments = 12);
void AddCircle3D(const Vec3& pos3D, float radius, ImColor color, int segments);
void AddText(int x0, int y0, const char* text, ImColor color, ImFontFlags flags = ImFontFlags_Shadow);
}

#endif // UTIL_DRAW_HPP
