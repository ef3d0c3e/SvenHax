#include "../Interface.hpp"
#include "../Util/Draw.hpp"
#include "Hooks.hpp"

#include "../Hacks/ESP.hpp"

#include <mutex>

std::mutex drawMutex;

void Hooks::PaintImGui()
{
	std::unique_lock<std::mutex> lock(drawMutex);


	int w, h;
	surface->GetScreenSize(w, h);
	float width = (float)w;
	float height = (float)h;
	float imWidth = SDL2::windowWidth;
	float imHeight = SDL2::windowHeight;

	ESP::Paint();

	for (const DrawRequest& value : Draw::drawRequests)
	{
		/* Convert in case there are stretched resolution users - DONT write to original struct! */
		int x0 = (int)((value.x0 / width) * imWidth);
		int y0 = (int)((value.y0 / height) * imHeight);
		int x1 = (int)((value.x1 / width) * imWidth);
		int y1 = (int)((value.y1 / height) * imHeight);

		switch (value.type)
		{
			case DRAW_LINE:
				Draw::ImLine(ImVec2(x0, y0), ImVec2(x1, y1), value.color);
				break;
			case DRAW_RECT:
				Draw::ImRect(x0, y0, x1, y1, value.color);
				break;
			case DRAW_RECT_FILLED:
				Draw::ImRectFilled(x0, y0, x1, y1, value.color);
				break;
			case DRAW_CIRCLE:
				Draw::ImCircle(ImVec2(x0, y0), value.color, value.circleRadius, value.circleSegments, value.thickness);
				break;
			case DRAW_CIRCLE_FILLED:
				Draw::ImCircleFilled(ImVec2(x0, y0), value.color, value.circleRadius, value.circleSegments);
				break;
			case DRAW_CIRCLE_3D:
				Draw::ImCircle3D(value.pos, value.circleSegments, value.circleRadius, value.color);
				break;
			case DRAW_TEXT:
				Draw::ImText(ImVec2(x0, y0), value.color, value.text, nullptr, 0.0f, nullptr, value.fontflags);
				break;
		}
	}
}
