#ifndef SDK_CVGUISYSTEMMODULELOADER_HPP
#define SDK_CVGUISYSTEMMODULELOADER_HPP

#include "IBaseInterface.hpp"

class CVGuiSystemModuleLoader : public IBaseInterface
{
public:
	virtual ~CVGuiSystemModuleLoader() = 0;

	virtual int GetModuleCount() = 0;
	virtual const char *GetModuleLabel(int moduleIndex) = 0;
	virtual CreateInterfaceFn GetModuleFactory(int moduleIndex) = 0;
	virtual bool ActivateModule(int moduleIndex) = 0;
	virtual bool ActivateModule(const char *moduleName) = 0;
	virtual void SetPlatformToRestart() = 0;
	virtual bool /*?*/ NotifyOnExitToDesktop() = 0;
};

#endif // SDK_CVGUISYSTEMMODULELOADER_HPP
