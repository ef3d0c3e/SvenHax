#ifndef UTIL_TRAITS_HPP
#define UTIL_TRAITS_HPP

#include <tuple>

template <typename T>
struct function_traits : public function_traits<decltype(&T::operator())>
{};

template <typename T, typename R, typename... Args>
struct function_traits<R (T::*)(Args...) const>
{
	static inline constexpr std::size_t arity = sizeof...(Args);

	typedef R return_type;

	template <std::size_t i> struct arg
	{
		typedef typename std::tuple_element<i, std::tuple<Args...>>::type type;
	};
};

#endif // UTIL_TRAITS_HPP
