#include "Hooks.hpp"
#include "../Interface.hpp"
#include <mutex>

int Paint::engineWidth;
int Paint::engineHeight;
int Paint::windowWidth;
int Paint::windowHeight;

std::mutex drawMutex;

typedef void (*PaintFn) (void*, PaintMode_t);
void Hooks::Paint(void* thisptr, PaintMode_t mode)
{
	//engineVGuiVMT->GetOriginalMethod<PaintFn>(10)(thisptr, mode);

    surface->GetScreenSize(Paint::engineWidth, Paint::engineHeight);
}
