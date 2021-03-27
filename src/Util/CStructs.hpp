#ifndef CSTRUCTS_HPP
#define CSTRUCTS_HPP

#include "Cenum.hpp"

#define __CSTRUCTS_CONCAT_(__a, __b) __a##__b
#define __CSTRUCTS_CONCAT(__a, __b) __CSTRUCTS_CONCAT_(__a,__b)
#define __CSTRUCTS_MAP(__counter_base, __name, __visibility, __m, __first, __second, __third, ...) \
	__m(__name, __visibility, __COUNTER__ - __counter_base - 1, __first, __second, __third)     \
		__CENUM_IF_ELSE(__CENUM_HAS_ARGS(__VA_ARGS__))(                \
			__CENUM_DEFER2(__CSTRUCTS__MAP)()(__counter_base, __name, __visibility, __m, __VA_ARGS__))()
#define __CSTRUCTS__MAP() __CSTRUCTS_MAP

#define __CSTRUCTS_CVALUES(__name, __visibility, __N, __tp, __id, __off) \
	constexpr static inline unsigned long long __id = __off;
#define __CSTRUCTS_IF_GET(__name, __visibility, __N, __tp, __id, __off) \
	else if constexpr (i == __N)               \
	{                                          \
		return __id;                           \
	}
#define __CSTRUCTS_IF_SZ(__name, __visibility, __N, __tp, __id, __off) \
	else if constexpr (i == __N)               \
	{                                          \
		return sizeof(__tp);                   \
	}
#define __CSTRUCTS_MEMBERS(__name, __visibility, __N, __tp, __id, __off) \
		struct { char8_t data[__CSTRUCTS_CONCAT(__name, __offsets)::getPadding<__N>()]; } __CSTRUCTS_CONCAT(__pad_, __id); \
		__tp __id;

#define CSTRUCTS_MEMBERS(__name, __visibility, ...)                                                         \
	struct __CSTRUCTS_CONCAT(__name, __offsets)                                                             \
	{                                                                                                       \
	private:                                                                                                \
		using __type = unsigned long long;                                                                  \
		using __cref = unsigned long long;                                                                  \
		using __ref = unsigned long long&;                                                                  \
		using __crv = const unsigned long long&&;                                                           \
		using __size_type = unsigned long long;                                                             \
                                                                                                            \
	public:                                                                                                 \
		__CENUM_EXPAND(__CENUM_EVAL(__CSTRUCTS_MAP(1, __name, __visibility, __CSTRUCTS_CVALUES, __VA_ARGS__))) \
                                                                                                            \
	private:                                                                                                \
		enum                                                                                                \
		{                                                                                                   \
			__counter1 = __COUNTER__                                                                        \
		};                                                                                                  \
                                                                                                            \
	public:                                                                                                 \
		template <__size_type i>                                                                            \
		constexpr static __type get()                                                                       \
		{                                                                                                   \
			if constexpr (false)                                                                            \
			{                                                                                               \
				return __type();                                                                            \
			}                                                                                               \
			__CENUM_EXPAND(__CENUM_EVAL(__CSTRUCTS_MAP(__counter1, __name, __visibility, __CSTRUCTS_IF_GET, __VA_ARGS__))) \
			else                                                                                            \
			{                                                                                               \
				[]<bool v = false>()                                                                        \
				{                                                                                           \
					static_assert(v, "Out of bounds");                                                      \
				}                                                                                           \
				();                                                                                         \
			}                                                                                               \
			return __type();                                                                                \
		}                                                                                                   \
		constexpr static inline __size_type size = __COUNTER__ - __counter1 - 1;                            \
                                                                                                            \
	private:                                                                                                \
		enum                                                                                                \
		{                                                                                                   \
			__counter2 = __COUNTER__                                                                        \
		};                                                                                                  \
                                                                                                            \
	public:                                                                                                 \
		template <__size_type i>                                                                            \
		consteval static __size_type sizeOf()                                                               \
		{                                                                                                   \
			if constexpr (false)                                                                            \
			{                                                                                               \
				return __size_type();                                                                       \
			}                                                                                               \
			__CENUM_EXPAND(__CENUM_EVAL(__CSTRUCTS_MAP(__counter2, __name, __visibility, __CSTRUCTS_IF_SZ, __VA_ARGS__))) \
			else                                                                                            \
			{                                                                                               \
				[]<bool v = false>()                                                                        \
				{                                                                                           \
					static_assert(v, "Out of bounds");                                                      \
				}                                                                                           \
				();                                                                                         \
			}                                                                                               \
			return __size_type();                                                                           \
		}                                                                                                   \
		                                                                                                    \
		template <__size_type i>                                                                            \
		constexpr static __size_type getPadding()                                                           \
		{                                                                                                   \
			if constexpr (i == 0)                                                                           \
			{                                                                                               \
				return get<i>();                                                                            \
			}                                                                                               \
			else if constexpr(i >= size)                                                                    \
			{                                                                                               \
				[]<bool v = false>()                                                                        \
				{                                                                                           \
					static_assert(v, "Out of bounds");                                                      \
				}                                                                                           \
				();                                                                                         \
			}                                                                                               \
			else                                                                                            \
			{                                                                                               \
				return get<i>() - (get<i-1>()+sizeOf<i-1>());                                               \
			}                                                                                               \
			return __size_type();                                                                           \
		}                                                                                                   \
	};                                                                                                      \
	struct __name                                                                                           \
	{                                                                                                       \
	__visibility:                                                                                           \
		enum                                                                                                \
		{                                                                                                   \
			__counter_base = __COUNTER__                                                                    \
		};                                                                                                  \
		__CENUM_EXPAND(__CENUM_EVAL(__CSTRUCTS_MAP(__counter_base, __name, __visibility, __CSTRUCTS_MEMBERS, __VA_ARGS__)))

#endif // CSTRUCTS_HPP
