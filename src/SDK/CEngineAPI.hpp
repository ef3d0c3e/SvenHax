#ifndef SDK_CENGINEAPI_HPP
#define SDK_CENGINEAPI_HPP

#include "IBaseInterface.hpp"

class CEngineAPI
{
public:
	virtual ~CEngineAPI() = 0;

	virtual void Run(void*, char*, char*, char*, IBaseInterface* (*)(char const*, int*), IBaseInterface* (*)(char const*, int*)) = 0;
};

#endif // SDK_CENGINEAPI_HPP
