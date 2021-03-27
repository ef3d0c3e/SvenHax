#ifndef UTIL_MATH_HPP
#define UTIL_MATH_HPP

#include <cstdint>
#include "Vector.hpp"

typedef std::uint8_t u8;
typedef std::uint16_t u16;
typedef std::uint32_t u32;
typedef std::uint64_t u64;
typedef std::int8_t i8;
typedef std::int16_t i16;
typedef std::int32_t i32;
typedef std::int64_t i64;
typedef float f32;
typedef double f64;
typedef Vector<f32, 2> v2f;
typedef Vector<f64, 2> v2d;

namespace Math
{
	std::pair<f32, f32> SinCos(f32 t);
}

#endif // UTIL_MATH_HPP
