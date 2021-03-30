#ifndef ENGINE_PRIMITIVES_QANGLE_HPP
#define ENGINE_PRIMITIVES_QANGLE_HPP

#include "../../Util/Vector.hpp"
#include "../../Util/Cenum.hpp"
#include "Defs.hpp"

typedef Vector<f32, 3> QAngle;
MAKE_CENUM_Q(QAngleType, u8, // TODO Check if this is the correct order
	PITCH, 0,
	YAW, 1,
	ROLL, 2,
);

#endif // ENGINE_PRIMITIVES_QANGLE_HPP
