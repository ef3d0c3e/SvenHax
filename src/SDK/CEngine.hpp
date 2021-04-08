#ifndef SDK_CENGINE_HPP
#define SDK_CENGINE_HPP

#include "../Engine/Primitives/Defs.hpp"

class CEngine
{
public:
	virtual ~CEngine() = 0;
	virtual void /*?*/ Load(bool, char*, char*) = 0;
	virtual void /*?*/ Unload() = 0;
	virtual void /*?*/ SetState(int) = 0;
	virtual void /*?*/ GetState() = 0;
	virtual void /*?*/ SetSubState(int) = 0;
	virtual void /*?*/ GetSubState() = 0;
	virtual void /*?*/ Frame() = 0;
	virtual void /*?*/ GetFrameTime() = 0;
	virtual f32 GetCurTime() = 0;
	virtual void /*?*/ TrapKey_Event(int, bool) = 0;
	virtual void /*?*/ TrapMouse_Event(int, bool) = 0;
	virtual void /*?*/ StartTrapMode() = 0;
	virtual void /*?*/ IsTrapping() = 0;
	virtual void /*?*/ CheckDoneTrapping(int&, int&) = 0;
	virtual void /*?*/ GetQuitting() = 0;
	virtual void /*?*/ SetQuitting(int) = 0;
};

#endif // SDK_CENGINE_HPP
