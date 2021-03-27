#ifndef SDK_CCLIENTDLL_HPP
#define SDK_CCLIENTDLL_HPP


#include "IBaseInterface.hpp"
struct ref_params_t;

class CCLientDLL
{
public:
	virtual ~CCLientDLL() = 0;

	virtual void /*?*/ Init(IBaseInterface* (*)(const char*, int*)) = 0;
	virtual void /*?*/ PlaySoundFX(float*, int, char*, float, float, int, int) = 0;
	virtual void /*?*/ RenderBegin(ref_params_t*) = 0;
	virtual void /*?*/ EndFrame() = 0;
};

#endif // SDK_CCLIENTDLL_HPP
