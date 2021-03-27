#ifndef SDK_CBASEUI_HPP
#define SDK_CBASEUI_HPP

#include "IBaseInterface.hpp"

class CBaseUI : public IBaseInterface
{
public:
	virtual ~CBaseUI() = 0;

	virtual void Initialize(CreateInterfaceFn *factories, int count) = 0;
	virtual void Start(struct cl_enginefuncs_s *engineFuncs, int interfaceVersion) = 0;
	virtual void Shutdown() = 0;

	virtual int Key_Event(int down, int keynum, const char *pszCurrentBinding) = 0;
	virtual void CallEngineSurfaceAppHandler(void *event, void *userData) = 0;
	virtual void Paint(int x, int y, int right, int bottom) = 0;
	virtual void HideGameUI() = 0;
	virtual void ActivateGameUI() = 0;
	virtual void HideConsole() = 0;
	virtual void ShowConsole() = 0;
};

#endif // SDK_CBASEUI_HPP
