/*	Cenum - zero-cost enum replacement
	Copyright (C) 2021  ef3d0c3e
	
	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	any later version.
	
	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.
	
	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <https://www.gnu.org/licenses/>. */

#ifndef CENUM_HPP
#define CENUM_HPP

#define __CENUM_EXPAND(...) __VA_ARGS__
#define __CENUM_FIRST(__a, ...) __a
#define __CENUM_SECOND(__a, __b, ...) __b
#define __CENUM_EMPTY()

#define __CENUM_EVAL(...) __CENUM_EVAL1024(__VA_ARGS__)
#define __CENUM_EVAL1024(...) __CENUM_EVAL512(__CENUM_EVAL512(__VA_ARGS__))
#define __CENUM_EVAL512(...) __CENUM_EVAL256(__CENUM_EVAL256(__VA_ARGS__))
#define __CENUM_EVAL256(...) __CENUM_EVAL128(__CENUM_EVAL128(__VA_ARGS__))
#define __CENUM_EVAL128(...) __CENUM_EVAL64(__CENUM_EVAL64(__VA_ARGS__))
#define __CENUM_EVAL64(...) __CENUM_EVAL32(__CENUM_EVAL32(__VA_ARGS__))
#define __CENUM_EVAL32(...) __CENUM_EVAL16(__CENUM_EVAL16(__VA_ARGS__))
#define __CENUM_EVAL16(...) __CENUM_EVAL8(__CENUM_EVAL8(__VA_ARGS__))
#define __CENUM_EVAL8(...) __CENUM_EVAL4(__CENUM_EVAL4(__VA_ARGS__))
#define __CENUM_EVAL4(...) __CENUM_EVAL2(__CENUM_EVAL2(__VA_ARGS__))
#define __CENUM_EVAL2(...) __CENUM_EVAL1(__CENUM_EVAL1(__VA_ARGS__))
#define __CENUM_EVAL1(...) __VA_ARGS__

#define __CENUM_DEFER1(__m) __m __CENUM_EMPTY()
#define __CENUM_DEFER2(__m) __m __CENUM_EMPTY __CENUM_EMPTY()()
#define __CENUM_DEFER3(__m) __m __CENUM_EMPTY __CENUM_EMPTY __CENUM_EMPTY()()()
#define __CENUM_DEFER4(__m) __m __CENUM_EMPTY __CENUM_EMPTY __CENUM_EMPTY __CENUM_EMPTY()()()()

#define __CENUM_IS_PROBE(...) __CENUM_SECOND(__VA_ARGS__, 0)
#define __CENUM_PROBE() ~, 1

#define __CENUM_CAT(__a, __b) __a##__b

#define __CENUM_NOT(__x) __CENUM_IS_PROBE(__CENUM_CAT(__CENUM__NOT_, __x))
#define __CENUM__NOT_0 __CENUM_PROBE()

#define __CENUM_BOOL(__x) __CENUM_NOT(__CENUM_NOT(__x))

#define __CENUM_IF_ELSE(__condition) __CENUM__IF_ELSE(__CENUM_BOOL(__condition))
#define __CENUM__IF_ELSE(__condition) __CENUM_CAT(__CENUM__IF_, __condition)

#define __CENUM__IF_1(...) __VA_ARGS__ __CENUM__IF_1_ELSE
#define __CENUM__IF_0(...) __CENUM__IF_0_ELSE

#define __CENUM__IF_1_ELSE(...)
#define __CENUM__IF_0_ELSE(...) __VA_ARGS__

#define __CENUM_HAS_ARGS(...) __CENUM_BOOL(__CENUM_FIRST(__CENUM__END_OF_ARGUMENTS_ __VA_ARGS__)())
#define __CENUM__END_OF_ARGUMENTS_() 0

#define __CENUM_MAP(__counter_base, __tp, __m, __first, __second, ...) \
	__m(__COUNTER__ - __counter_base - 1, __tp, __first, __second)     \
		__CENUM_IF_ELSE(__CENUM_HAS_ARGS(__VA_ARGS__))(                \
			__CENUM_DEFER2(__CENUM__MAP)()(__counter_base, __tp, __m, __VA_ARGS__))()
#define __CENUM__MAP() __CENUM_MAP

#define __CENUM_CVALUES(__N, __tp, __id, __val) \
	constexpr static inline __tp __id = __val;
#define __CENUM_IF_ORD(__N, __tp, __id, __val) \
	else if constexpr (x == __id)              \
	{                                          \
		return __N;                            \
	}
#define __CENUM_IF_GET(__N, __tp, __id, __val) \
	else if constexpr (i == __N)               \
	{                                          \
		return __id;                           \
	}
#define MAKE_CENUMV_Q(__enum_name, __enum_type, ...) \
	MAKE_CENUMV(__enum_name, __enum_type, const __enum_type&, __enum_type&, const __enum_type&&, unsigned long long, __VA_ARGS__)
#define MAKE_CENUMV(__enum_name, __enum_type, __enum_cref, __enum__ref, __enum_crv, __enum_sz_tp, ...) \
	__CENUM_EXPAND(__CENUM_EVAL(__CENUM_MAP(1, __enum_type, __CENUM_CVALUES, __VA_ARGS__)))            \
	MAKE_CENUM(__enum_name, __enum_type, __enum_cref, __enum__ref, __enum_crv, __enum_sz_tp, __VA_ARGS__)
#define MAKE_CENUM_Q(__enum_name, __enum_type, ...) \
	MAKE_CENUM(__enum_name, __enum_type, const __enum_type&, __enum_type&, const __enum_type&&, unsigned long long, __VA_ARGS__)
#define MAKE_CENUM(__enum_name, __enum_type, __enum_cref, __enum__ref, __enum_crv, __enum_sz_tp, ...)       \
	struct __enum_name                                                                                      \
	{                                                                                                       \
	private:                                                                                                \
		using __type = __enum_type;                                                                         \
		using __cref = __enum_cref;                                                                         \
		using __ref = __enum_cref;                                                                          \
		using __crv = __enum_crv;                                                                           \
		using __size_type = __enum_sz_tp;                                                                   \
                                                                                                            \
	public:                                                                                                 \
		__type value;                                                                                       \
                                                                                                            \
		constexpr __enum_name() noexcept                                                                    \
		: value(get<0>())                                                                                   \
		{}                                                                                                  \
		constexpr __enum_name(auto v) noexcept                                                              \
		: value(v)                                                                                          \
		{}                                                                                                  \
		constexpr __enum_name(__ref v) noexcept                                                             \
		: value(v)                                                                                          \
		{}                                                                                                  \
		constexpr __enum_name(__crv v) noexcept                                                             \
		: value(v)                                                                                          \
		{}                                                                                                  \
                                                                                                            \
		constexpr operator __type() const noexcept                                                          \
		{                                                                                                   \
			return value;                                                                                   \
		}                                                                                                   \
                                                                                                            \
		constexpr __enum_name& operator=(__cref v) noexcept                                                 \
		{                                                                                                   \
			value = v;                                                                                      \
			return *this;                                                                                   \
		}                                                                                                   \
                                                                                                            \
		constexpr __enum_name& operator=(__crv v) noexcept                                                  \
		{                                                                                                   \
			value = v;                                                                                      \
			return *this;                                                                                   \
		}                                                                                                   \
                                                                                                            \
		constexpr __enum_name& operator=(auto v) noexcept                                                   \
		{                                                                                                   \
			value = v;                                                                                      \
			return *this;                                                                                   \
		}                                                                                                   \
                                                                                                            \
		constexpr __enum_name& operator~() noexcept                                                         \
		{                                                                                                   \
			value = ~value;                                                                                 \
			return *this;                                                                                   \
		}                                                                                                   \
                                                                                                            \
		constexpr __enum_name& operator|=(auto v) noexcept                                                  \
		{                                                                                                   \
			value |= v;                                                                                     \
			return *this;                                                                                   \
		}                                                                                                   \
                                                                                                            \
		constexpr __enum_name& operator&=(auto v) noexcept                                                  \
		{                                                                                                   \
			value &= v;                                                                                     \
			return *this;                                                                                   \
		}                                                                                                   \
                                                                                                            \
		constexpr bool operator==(const __enum_name& e) const noexcept                                      \
		{                                                                                                   \
			return value == e.value;                                                                        \
		}                                                                                                   \
                                                                                                            \
		constexpr bool operator==(const __enum_name&& e) const noexcept                                     \
		{                                                                                                   \
			return value == e.value;                                                                        \
		}                                                                                                   \
                                                                                                            \
		constexpr bool operator==(const auto& v) const noexcept                                             \
		{                                                                                                   \
			return value == v;                                                                              \
		}                                                                                                   \
                                                                                                            \
		constexpr bool operator==(const auto&& v) const noexcept                                            \
		{                                                                                                   \
			return value == v;                                                                              \
		}                                                                                                   \
                                                                                                            \
		constexpr bool operator!=(const __enum_name& e) const noexcept                                      \
		{                                                                                                   \
			return value != e.value;                                                                        \
		}                                                                                                   \
                                                                                                            \
		constexpr bool operator!=(const __enum_name&& e) const noexcept                                     \
		{                                                                                                   \
			return value != e.value;                                                                        \
		}                                                                                                   \
                                                                                                            \
		constexpr bool operator!=(const auto& v) const noexcept                                             \
		{                                                                                                   \
			return value != v;                                                                              \
		}                                                                                                   \
                                                                                                            \
		constexpr bool operator!=(const auto&& v) const noexcept                                            \
		{                                                                                                   \
			return value != v;                                                                              \
		}                                                                                                   \
                                                                                                            \
		__CENUM_EXPAND(__CENUM_EVAL(__CENUM_MAP(1, __enum_type, __CENUM_CVALUES, __VA_ARGS__)))             \
                                                                                                            \
	private:                                                                                                \
		enum                                                                                                \
		{                                                                                                   \
			__counter1 = __COUNTER__                                                                        \
		};                                                                                                  \
                                                                                                            \
	public:                                                                                                 \
		template <__type x>                                                                                 \
		constexpr static __size_type ord()                                                                  \
		{                                                                                                   \
			if constexpr (false)                                                                            \
			{                                                                                               \
				return __size_type();                                                                       \
			}                                                                                               \
			__CENUM_EXPAND(__CENUM_EVAL(__CENUM_MAP(__counter1, __enum_type, __CENUM_IF_ORD, __VA_ARGS__))) \
			else                                                                                            \
			{                                                                                               \
				[]<bool v = false>()                                                                        \
				{                                                                                           \
					static_assert(v, "Out of range");                                                       \
				}                                                                                           \
				();                                                                                         \
			}                                                                                               \
		}                                                                                                   \
                                                                                                            \
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
		constexpr static __type get()                                                                       \
		{                                                                                                   \
			if constexpr (false)                                                                            \
			{                                                                                               \
				return __type();                                                                            \
			}                                                                                               \
			__CENUM_EXPAND(__CENUM_EVAL(__CENUM_MAP(__counter2, __enum_type, __CENUM_IF_GET, __VA_ARGS__))) \
			else                                                                                            \
			{                                                                                               \
				[]<bool v = false>()                                                                        \
				{                                                                                           \
					static_assert(v, "Out of bounds");                                                      \
				}                                                                                           \
				();                                                                                         \
			}                                                                                               \
		}                                                                                                   \
                                                                                                            \
	private:                                                                                                \
		template <__size_type i, __size_type End, __size_type Inc, template <__size_type> class Fn>         \
		static inline void iterate_impl()                                                                   \
		{                                                                                                   \
			if constexpr (i >= End)                                                                         \
				return;                                                                                     \
			else                                                                                            \
			{                                                                                               \
                                                                                                            \
				Fn<i> f;                                                                                    \
				f();                                                                                        \
                                                                                                            \
				iterate_impl<i + Inc, End, Inc, Fn>();                                                      \
			}                                                                                               \
		}                                                                                                   \
                                                                                                            \
	public:                                                                                                 \
		template <__size_type Beg, __size_type End, __size_type Inc, template <__size_type> class Fn>       \
		static void iterate()                                                                               \
		{                                                                                                   \
			static_assert(Beg <= End);                                                                      \
			static_assert(End <= size);                                                                     \
			static_assert(Inc != 0);                                                                        \
                                                                                                            \
			iterate_impl<Beg, End, Inc, Fn>();                                                              \
		}                                                                                                   \
	}

#endif // CENUM_HPP
