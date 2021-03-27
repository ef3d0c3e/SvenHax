#ifndef SDK_CDEFAULTCVAR_HPP
#define SDK_CDEFAULTCVAR_HPP

#include "IBaseInterface.hpp"
class ConCommandBase;
class ConVar;
typedef void (*FnChangeCallback_t)(ConVar*, char const*);

class CDefaultCvar
{
public:
	//virtual ~CDefaultCvar() = 0;
	virtual bool RegisterConCommandBase(ConCommandBase* pVar) = 0;
	virtual const char* GetCommandLineValue(const char* pVarName) = 0;
	virtual ConVar* FindVar(const char* var_name) = 0;
	virtual const ConVar* FindVar(const char* var_name) const = 0;
	virtual const char* /*?*/ GetCommands() = 0;
	virtual bool Connect(CreateInterfaceFn factory) = 0;
	virtual void Disconnect() = 0;
	virtual void* QueryInterface(const char* pInterfaceName) = 0;
	virtual void __Disconnect() = 0;
	virtual void /*?*/ UnlinkVariables(int) = 0;
	virtual void InstallGlobalChangeCallback(FnChangeCallback_t callback) = 0;
	virtual void CallGlobalChangeCallbacks(ConVar* var, const char* pOldString, float flOldValue) = 0;
};

#endif // SDK_CDEFAULTCVAR_HPP
