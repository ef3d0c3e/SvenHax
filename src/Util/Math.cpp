#include "Math.hpp"


std::pair<f32, f32> Math::SinCos(f32 t)
{
	f32 cosr, sinr;
	__asm ("fsincos" : "=t" (cosr), "=u" (sinr) : "0" (t));

	return {sinr, cosr};
}
