#ifndef SDK_CGAMECONSOLE_HPP
#define SDK_CGAMECONSOLE_HPP

#include "IEngineVGui.hpp"


class CGameConsole : public IBaseInterface
{
public:
	virtual ~CGameConsole() = 0;

	virtual void Activate() = 0;
	virtual void Initialize() = 0;
	virtual void Hide() = 0;

	// clears the console
	virtual void Clear() = 0;

	// return true if the console has focus
	virtual bool IsConsoleVisible() = 0;
	virtual void Printf(const char *format, ...) = 0;
	virtual void DPrintf(const char *format, ...) = 0;

	virtual void SetParent(VPANEL parent) = 0;
};

#endif // SDK_CGAMECONSOLE_HPP
