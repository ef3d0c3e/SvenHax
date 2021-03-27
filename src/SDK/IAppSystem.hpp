#ifndef SDK_IAPPSYSTEM_HPP
#define SDK_IAPPSYSTEM_HPP

#include "../Util/Cenum.hpp"
#include "IBaseInterface.hpp"

MAKE_CENUMV_Q(InitReturnVal_t, int,
	INIT_FAILED, 0,
	INIT_OK, 1,

	INIT_LAST_VAL, 2,
);

class IAppSystem
{
public:
};

#endif // SDK_IAPPSYSTEM_HPP
