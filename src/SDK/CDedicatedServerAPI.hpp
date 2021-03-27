#ifndef SDK_CDEDICATEDSERVERAPI_HPP
#define SDK_CDEDICATEDSERVERAPI_HPP

#include "IBaseInterface.hpp"

class CDedicatedServerAPI
{
public:
	virtual ~CDedicatedServerAPI() = 0;
	virtual void /*?*/ Init(char*, char*, IBaseInterface* (*)(char const*, int*), IBaseInterface* (*)(char const*, int*)) = 0;
	virtual void /*?*/ Shutdown() = 0;
	virtual void /*?*/ RunFrame() = 0;
	virtual void /*?*/ AddConsoleText(char*) = 0;
};

#endif // SDK_CDEDICATEDSERVERAPI_HPP
