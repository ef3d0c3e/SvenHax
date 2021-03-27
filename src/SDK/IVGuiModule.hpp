#ifndef SDK_IVGUIMODULE_HPP
#define SDK_IVGUIMODULE_HPP

#include "IBaseInterface.hpp"
#include "CEngineVGui.hpp" // VPANEL

class IVGuiModule : public IBaseInterface
{
public:
	virtual ~IVGuiModule() = 0;

	// called first to setup the module with the vgui
	// returns true on success, false on failure
	virtual bool Initialize(CreateInterfaceFn *vguiFactories, int factoryCount) = 0;

	// called after all the modules have been initialized
	// modules should use this time to link to all the other module interfaces
	virtual bool PostInitialize(CreateInterfaceFn *modules = nullptr, int factoryCount = 0) = 0;

	// called when the module is selected from the menu or otherwise activated
	virtual bool Activate() = 0;

	// returns true if the module is successfully initialized and available
	virtual bool IsValid() = 0;

	// requests that the UI is temporarily disabled and all data files saved
	virtual void Deactivate() = 0;

	// restart from a Deactivate()
	virtual void Reactivate() = 0;

	// called when the module is about to be shutdown
	virtual void Shutdown() = 0;

	// returns a handle to the main module panel
	virtual VPANEL GetPanel() = 0;

	// sets the parent of the main module panel
	virtual void SetParent(VPANEL parent) = 0;

	virtual void OnExitToDesktop() = 0;

	// messages sent through through the panel returned by GetPanel():
	//
	//  "ConnectedToGame"	"ip" "port" "gamedir"
	//	"DisconnectedFromGame"
	//  "ActiveGameName"	"name"
	//	"LoadingStarted"	"type"	"name"
	//  "LoadingFinished"	"type"	"name"

	virtual void RunFrame() = 0;
};

#endif // SDK_IVGUIMODULE_HPP
