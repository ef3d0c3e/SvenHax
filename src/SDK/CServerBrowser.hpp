#ifndef SDK_CSERVERBROWSER_HPP
#define SDK_CSERVERBROWSER_HPP

#include "IServerBrowser.hpp"
#include "IVGuiModule.hpp"

class CServerBrowser : public IServerBrowser, public IVGuiModule
{
public:
	~CServerBrowser() = 0;

	// IVGui module implementation
	bool Initialize(CreateInterfaceFn *factorylist, int numFactories) = 0;
	bool PostInitialize(CreateInterfaceFn *modules, int factoryCount) = 0;
	bool Activate() = 0;
	bool IsValid() = 0;
	void Deactivate() = 0;
	void Reactivate() = 0;
	void OnExitToDesktop() = 0;
	void Shutdown() = 0;
	VPANEL GetPanel() = 0;
	void SetParent(VPANEL parent) = 0;
	virtual void RunFrame() = 0;


};

#endif // SDK_CSERVERBROWSER_HPP
