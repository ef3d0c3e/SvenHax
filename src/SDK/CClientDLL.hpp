#ifndef SDK_CCLIENTDLL_HPP
#define SDK_CCLIENTDLL_HPP


#include "IBaseInterface.hpp"
#include "../Engine/Primitives/RefParams.hpp"

class CCLientDLL
{
public:
	virtual ~CCLientDLL() = 0;

	virtual void /*?*/ Init(IBaseInterface* (*)(const char*, int*)) = 0;
	virtual void /*?*/ PlaySoundFX(float*, int, char*, float, float, int, int) = 0;
	virtual void /*?*/ RenderBegin(RefParams*) = 0;
	virtual void /*?*/ RenderEnd() = 0;
};

#endif // SDK_CCLIENTDLL_HPP
