#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <cmath>
#include <functional>
#include <iostream>
#include <type_traits>
#include <utility>
#include <vector>

#if defined(__GNUC__)
#define __vector_inline __attribute__((always_inline))
#else
#define __vector_inline inline
#endif

#define __expand(...) __VA_ARGS__
#define vector_foreach_do_v(expr, __no_parallel, __N)                                                                            \
	{                                                                                                                            \
		if constexpr (S.UseFor)                                                                                                  \
		{                                                                                                                        \
			std::size_t i;                                                                                                       \
			if constexpr (!__no_parallel && S.UseParallel)                                                                       \
			{                                                                                                                    \
				if constexpr (S.NumThreads != 0)                                                                                 \
				{                                                                                                                \
					_Pragma("omp parallel for private(i) schedule(dynamic) num_threads(S.NumThreads)") for (i = 0; i < __N; ++i) \
					{                                                                                                            \
						(__expand(expr));                                                                                        \
					}                                                                                                            \
				}                                                                                                                \
				else                                                                                                             \
				{                                                                                                                \
					_Pragma(                                                                                                     \
						"omp parallel for private(i) schedule(dynamic) ") for (i = 0;                                            \
																			   i < __N;                                          \
																			   ++i)                                              \
					{                                                                                                            \
						(__expand(expr));                                                                                        \
					}                                                                                                            \
				}                                                                                                                \
			}                                                                                                                    \
			else                                                                                                                 \
			{                                                                                                                    \
				if constexpr (S.UseSIMD)                                                                                         \
				{                                                                                                                \
					_Pragma(                                                                                                     \
						"omp simd safelen(S.SIMDSafeLen) simdlen(S.SIMDLen)") for (i = 0;                                        \
																				   i <                                           \
																				   __N;                                          \
																				   ++i)                                          \
					{                                                                                                            \
						(__expand(expr));                                                                                        \
					}                                                                                                            \
				}                                                                                                                \
				else                                                                                                             \
				{                                                                                                                \
					for (i = 0; i < __N; ++i)                                                                                    \
					{                                                                                                            \
						(__expand(expr));                                                                                        \
					}                                                                                                            \
				}                                                                                                                \
			}                                                                                                                    \
		}                                                                                                                        \
		else                                                                                                                     \
		{                                                                                                                        \
			[&]<std::size_t... i>(std::index_sequence<i...>)                                                                     \
			{                                                                                                                    \
				((__expand(expr)), ...);                                                                                         \
			}                                                                                                                    \
			(std::make_index_sequence<__N>{});                                                                                   \
		}                                                                                                                        \
	}
#define vector_foreach_do(expr) vector_foreach_do_v(expr, false, N)

struct VectorSettings
{
	////////////////////////////////////////////////////////////
	/// \class VectorSettings
	/// \brief Settings for the vector class
	/// \brief Settings modify how operations are done (operators, etc...)
	////////////////////////////////////////////////////////////

	bool UseFor = false; ///< Use for loop instead of (force) unrolled loop

	bool UseParallel = true; ///< Use parallelization (The overhead is often not worth the cost)
	int NumThreads = 0; ///< Number of threads for parallelization (0 = all cpu threads)

	bool UseSIMD = false; ///< Hints the compiler to use SIMD instruction inside the for loop
	int SIMDSafeLen = 0; ///< Minimum distance for splitting the logical iteration space
	int SIMDLen = 4; ///< Preferred number of concurrent iterations
};

/** @cond */
#define __vector_data_methods                                \
	__vector_inline T& operator[](std::size_t i)             \
	{                                                        \
		return *(reinterpret_cast<T*>(this)+i);              \
	}                                                        \
                                                             \
	__vector_inline const T& operator[](std::size_t i) const \
	{                                                        \
		return *(reinterpret_cast<const T*>(this)+i);        \
	}

template <class T, std::size_t N>
struct __vector_data
{
	T x, y, z, w, rest[N-4];

	__vector_data_methods;
};

template <class T>
struct __vector_data<T, 4>
{
	T x, y, z, w;

	__vector_data_methods;
};

template <class T>
struct __vector_data<T, 3>
{
	T x, y, z;

	__vector_data_methods;
};

template <class T>
struct __vector_data<T, 2>
{
	T x, y;

	__vector_data_methods;
};

template <class T>
struct __vector_data<T, 1>
{
	T x;

	__vector_data_methods;
};

template <class T>
struct __vector_data<T, 0>
{
	__vector_data_methods;
};
#undef __vector_data_methods
/** @endcond */

template <class T, std::size_t N, template <class _T, std::size_t _N> class B>
concept VectorBase = sizeof(T)*N == sizeof(B<T, N>) &&
requires(B<T, N>& b, std::size_t i)
{
	{ static_cast<const B<T, N>&>(b)[i] } -> std::same_as<const T&>;
	{ b[i] } -> std::same_as<T&>;
};

template <class T, std::size_t N, template <class, std::size_t> class B = __vector_data, VectorSettings S = VectorSettings{}>
requires VectorBase<T, N, B>
class Vector : public B<T, N>
{
	////////////////////////////////////////////////////////////
	/// \class Vector
	/// \brief Vector class based around constexpr for loop
	/// from C++20
	/// \brief The code generated for each operation is
	/// designed to be easily optimized
	/// through vectorization instructions
	/// \tparam T The type of every elements in the
	/// vector (usually int or double). Also called 'scalar'
	/// \tparam N The number of elements of the vector, or
	/// 'dimension' of the vector
	/// \tparam B The base type of the vector (see VectorBase)
	/// \tparam S The vector's settings
	////////////////////////////////////////////////////////////
	using base_type = B<T, N>;

public:
	////////////////////////////////////////////////////////////
	/// \brief Default constructor
	///
	/// Initializes all elements of the vector to T().
	///
	////////////////////////////////////////////////////////////
	/** @cond */ __vector_inline /** @endcond */
		constexpr Vector() noexcept
	{}

	////////////////////////////////////////////////////////////
	/// \brief Copy constructor
	///
	/// Initializes all elements of the vector to elements of v.
	///
	/// \param v Vector to copy from
	////////////////////////////////////////////////////////////
	/** @cond */ __vector_inline /** @endcond */
		constexpr Vector(Vector& v) noexcept
	{
		vector_foreach_do(operator[](i) = v[i]);
	}

	////////////////////////////////////////////////////////////
	/// \brief Copy constructor
	///
	/// Initializes all elements of the vector to elements of v.
	///
	/// \param v Vector to copy from
	////////////////////////////////////////////////////////////
	/** @cond */ __vector_inline /** @endcond */
		constexpr Vector(const Vector& v) noexcept
	{
		vector_foreach_do(operator[](i) = v[i]);
	}

	////////////////////////////////////////////////////////////
	/// \brief Move constructor
	///
	/// Moves all elements of v into itself.
	///
	/// \param v Vector to move from
	////////////////////////////////////////////////////////////
	/** @cond */ __vector_inline /** @endcond */
		constexpr Vector(Vector&& v) noexcept
	{
		vector_foreach_do(operator[](i) = std::move(v[i]));
	}

	////////////////////////////////////////////////////////////
	/// \brief Move constructor
	///
	/// Moves all elements of v into itself.
	///
	/// \param v Vector to move from
	////////////////////////////////////////////////////////////
	/** @cond */ __vector_inline /** @endcond */
		constexpr Vector(const Vector&& v) noexcept
	{
		vector_foreach_do(operator[](i) = std::move(v[i]));
	}

	////////////////////////////////////////////////////////////
	/// \brief Conversion
	///
	/// Convert elements from one scalar type to the other.
	///
	/// \tparam R The new scalar type
	////////////////////////////////////////////////////////////
	template <typename R>
	explicit operator Vector<R, N, B, S>() const noexcept
	{
		Vector<R, N, B, S> r;
		vector_foreach_do(r[i] = static_cast<R>(operator[](i)));
		return r;
	}

	////////////////////////////////////////////////////////////
	/// \brief std::initializer_list constructor
	///
	/// Initializes each of the vector's elements with the
	/// values contained in list.
	///
	/// \param list List of elements
	////////////////////////////////////////////////////////////
	/** @cond */ __vector_inline /** @endcond */
		constexpr Vector(const std::initializer_list<T>& list) noexcept
	{
		auto it = list.begin();
		vector_foreach_do_v(operator[](i) = *(it++), true, N);
	}

	////////////////////////////////////////////////////////////
	/// \brief std::array constructor
	///
	/// Initializes each of the vector's elements with The
	/// values contained in arr.
	///
	/// \param arr The array to initialize from
	////////////////////////////////////////////////////////////
	/** @cond */ __vector_inline /** @endcond */
		constexpr Vector(const std::array<T, N>& arr) noexcept
	{
		vector_foreach_do(operator[](i) = arr[i]);
	}

	////////////////////////////////////////////////////////////
	/// \brief Array constructor
	///
	/// Initializes each of the vector's elements with The
	/// values contained in arr.
	///
	/// \param arr The array to initialize from, its size must
	/// be greater or equal to N
	////////////////////////////////////////////////////////////
	/** @cond */ __vector_inline /** @endcond */
		constexpr Vector(const T* arr) noexcept
	{
		vector_foreach_do(operator[](i) = arr[i]);
	}

	auto cbegin() const noexcept
	{
		return reinterpret_cast<const T*>(this);
	}

	auto begin() noexcept
	{
		return reinterpret_cast<T*>(this);
	}

	const auto cend() const noexcept
	{
		return reinterpret_cast<const T*>(this) + N;
	}

	auto end() noexcept
	{
		return reinterpret_cast<T*>(this) + N;
	}

	const auto crbegin() const noexcept
	{
		return reinterpret_cast<const T*>(this) + N - 1;
	}

	auto rbegin() noexcept
	{
		return reinterpret_cast<T*>(this) + N - 1;
	}

	auto crend() const noexcept
	{
		return reinterpret_cast<const T*>(this) - 1;
	}

	auto rend() noexcept
	{
		return reinterpret_cast<T*>(this) - 1;
	}

	////////////////////////////////////////////////////////////
	/// \brief Size method
	///
	/// \returns The number of elements in the vector: N
	////////////////////////////////////////////////////////////
	/** @cond */ __vector_inline /** @endcond */
		constexpr std::size_t
		Size() const noexcept
	{
		return N;
	}


	////////////////////////////////////////////////////////////
	/// \brief Set method
	///
	/// \param v The value to fill the vector with
	///
	////////////////////////////////////////////////////////////
	/** @cond */ __vector_inline /** @endcond */
		void
		Set(T v) noexcept { vector_foreach_do(operator[](i) = v) }

	////////////////////////////////////////////////////////////
	/// \brief Clear method
	///
	/// Sets all elements of the vector to T()
	////////////////////////////////////////////////////////////
	/** @cond */ __vector_inline /** @endcond */
		void Clear() noexcept { vector_foreach_do(operator[](i) = T()) }


	////////////////////////////////////////////////////////////
	/// \brief Assignment operator
	///
	/// Copies each elements of v into itself.
	///
	/// \param v The vector to copy from
	////////////////////////////////////////////////////////////
	/** @cond */ __vector_inline /** @endcond */
		Vector&
		operator=(const Vector& v) noexcept
	{
		vector_foreach_do(operator[](i) = v[i]);
		return *this;
	}

	////////////////////////////////////////////////////////////
	/// \brief Equal operator
	///
	/// Checks if v and u are equal.
	///
	/// \param v Left operand
	/// \param u Right operand
	/// \returns true if all elements of v and u are equal
	/// (using T::operator==())
	////////////////////////////////////////////////////////////
	/** @cond */ __vector_inline /** @endcond */
		friend bool
		operator==(const Vector& v, const Vector& u) noexcept
	{
		return [&]<std::size_t... i>(std::index_sequence<i...>)
		{
			return ((v[i] == u[i]) && ...); // shortcut
		}
		(std::make_index_sequence<N>{});
	}

	////////////////////////////////////////////////////////////
	/// \brief Not equal operator
	///
	/// Checks if v and u are not equal.
	///
	/// \param v Left operand
	/// \param u Right operand
	/// \returns true if at least one element of v differs from
	/// u (using T::operator!=())
	////////////////////////////////////////////////////////////
	/** @cond */ __vector_inline /** @endcond */
		friend bool
		operator!=(const Vector& v, const Vector& u) noexcept
	{
		return [&]<std::size_t... i>(std::index_sequence<i...>)
		{
			return ((v[i] != u[i]) || ...); // shortcut
		}
		(std::make_index_sequence<N>{});
	}

	////////////////////////////////////////////////////////////
	/// \brief Put to stream operator
	///
	/// Puts all the elements of v into a stream.
	///
	/// \param stream The stream
	/// \param v The vector
	////////////////////////////////////////////////////////////
	template <class CharT>
	/** @cond */ __vector_inline /** @endcond */
		friend std::basic_ostream<CharT>&
		operator<<(std::basic_ostream<CharT>& stream, const Vector& v) noexcept
	{
		stream << '(';
		vector_foreach_do_v(stream << v[i] << ',' << ' ', true, N - 1) stream
			<< v[N - 1];
		stream << ')';

		return stream;
	}

	////////////////////////////////////////////////////////////
	/// \brief Constant access operator
	///
	/// Returns the element at the i-th position.
	///
	/// \warning Performs no bound checking
	/// \param i The position to get the element from
	////////////////////////////////////////////////////////////
	/** @cond */ __vector_inline /** @endcond */
		const T&
		operator[](const std::size_t i) const noexcept
	{
		return base_type::operator[](i);
	}

	////////////////////////////////////////////////////////////
	/// \brief Access operator
	///
	/// Returns the element at the i-th position.
	///
	/// \warning Performs no bound checking
	/// \param i The position to get the element from
	////////////////////////////////////////////////////////////
	/** @cond */ __vector_inline /** @endcond */
		T&
		operator[](const std::size_t i) noexcept
	{
		return base_type::operator[](i);
	}

	////////////////////////////////////////////////////////////
	/// \brief Apply a function to each elements
	///
	/// Apply fn to every elements of the vector.
	///
	/// \param fn The function to apply
	////////////////////////////////////////////////////////////
	/** @cond */ __vector_inline /** @endcond */
		void
		Apply(std::function<T(T)> fn) noexcept
	{
		vector_foreach_do(operator[](i) = fn(operator[](i)));
	}

	////////////////////////////////////////////////////////////
	/// \brief Reduce the vector to one T
	///
	/// Apply fn to every 2 elements of the vector:
	/// v(x, y, z) -> fn(fn(x, y), z), with fn(a, b) := a+b,
	/// this yields the sum of every elements of the vector.
	///
	/// \param fn The function to use for reducing
	/// \tparam noParallel Override ```.UseParallel```
	/// \returns A single T, containing the reduced input
	////////////////////////////////////////////////////////////
	template <bool noParallel = false>
	/** @cond */ __vector_inline /** @endcond */
		T
		Reduce(std::function<T(T, T)> fn) const noexcept
	{
		T r = operator[](0);
		vector_foreach_do_v((r = fn(r, operator[](i+1))), noParallel, N - 1)

			return r;
	}

	////////////////////////////////////////////////////////////
	/// \brief Get a single T using composition of boolean
	///
	/// Will compare every elements of the vector using fn.
	/// \code{.cpp}
	/// Vector<int, 5> v{3, 4, 7, -4, 1};
	/// v.Get([](int x, int y){ return x > y;}); // Will return the biggest element of v
	/// \endcode
	///
	/// \param fn The function to use for comparing
	/// \returns The resulting T
	////////////////////////////////////////////////////////////
	/** @cond */ __vector_inline /** @endcond */
		T
		Get(std::function<bool(T, T)> fn) const noexcept
	{
		T ret = operator[](0);
		vector_foreach_do(ret = (fn(ret, operator[](i))) ? operator[](i) : ret);

		return ret;
	}

	////////////////////////////////////////////////////////////
	/// \brief Get a list of T matching
	///
	/// Will return avery elements of the vector that matched
	/// the criteria fn
	/// \code{.cpp}
	/// Vector<int, 6> v{0, 3, 7, -4, 8, 2};
	/// auto div2 = v.Select([](int x){ return x % 2 == 0;}); // Will return all multiples of 2
	/// \endcode
	///
	/// \param fn The function to use for matching
	/// \returns A list of element that matched
	////////////////////////////////////////////////////////////
	/** @cond */ __vector_inline /** @endcond */
		std::vector<T>
		Select(std::function<bool(T)> fn) const noexcept
	{
		std::vector<T> r;

		vector_foreach_do((fn(operator[](i))) ? r.push_back(operator[](i)) : void());

		return r;
	}

#define OPERATOR(op)                                     \
	/** @cond */ __vector_inline /** @endcond */         \
		Vector                                           \
		operator op(Vector v) const noexcept             \
	{                                                    \
		vector_foreach_do(v[i] = operator[](i) op v[i]); \
		return v;                                        \
	}                                                    \
	/** @cond */ __vector_inline /** @endcond */         \
		Vector&                                          \
		operator op##=(const Vector& v) noexcept         \
	{                                                    \
		vector_foreach_do(operator[](i) op## = v[i]);    \
		return *this;                                    \
	}                                                    \
	/** @cond */ __vector_inline /** @endcond */         \
		Vector                                           \
		operator op(T k) const noexcept                  \
	{                                                    \
		Vector v(*this);                                 \
		vector_foreach_do(v[i] op## = k);                \
		return v;                                        \
	}                                                    \
	/** @cond */ __vector_inline /** @endcond */         \
		Vector                                           \
		friend operator op(T k, Vector v) noexcept       \
	{                                                    \
		vector_foreach_do(v[i] op## = k);                \
		return v;                                        \
	}                                                    \
	/** @cond */ __vector_inline /** @endcond */         \
		Vector&                                          \
		operator op##=(T k) noexcept                     \
	{                                                    \
		vector_foreach_do(operator[](i) op## = k);       \
		return *this;                                    \
	}

	OPERATOR(+);
	OPERATOR(-);
	OPERATOR(*);
	OPERATOR(/);
	OPERATOR(^);
	OPERATOR(&);
	OPERATOR(|);
	OPERATOR(<<);
	OPERATOR(>>);

	/** @cond */ __vector_inline /** @endcond */
		Vector
		operator-() const noexcept
	{
		Vector v(*this);
		vector_foreach_do(v[i] = -v[i]);
		return v;
	}

	/** @cond */ __vector_inline /** @endcond */
		Vector
		operator%(Vector v) const noexcept
	{
		vector_foreach_do(v[i] = operator[](i) % v[i]);
		return v;
	}
	/** @cond */ __vector_inline /** @endcond */
		Vector&
		operator%=(const Vector& v) noexcept
	{
		vector_foreach_do(operator[](i) = operator[](i) % v[i]);
		return *this;
	}
	/** @cond */ __vector_inline /** @endcond */
		Vector
		operator%(T k) const noexcept
	{
		Vector v(*this);
		vector_foreach_do(v[i] = operator[](i) % k);
		return v;
	}
	/** @cond */ __vector_inline /** @endcond */
		Vector
		friend operator%(T k, Vector v) noexcept
	{
		vector_foreach_do(v[i] = operator[](i) % k);
		return v;
	}
	/** @cond */ __vector_inline /** @endcond */
		Vector&
		operator%=(T k) noexcept
	{
		vector_foreach_do(operator[](i) = operator[](i) % k);
		return *this;
	}
#undef OPERATOR

	////////////////////////////////////////////////////////////
	/// \brief Euclidian distance
	///
	/// Will calculate the euclidian distance (or norm 2) of
	/// the vector
	///
	/// \code{.cpp}
	/// Vector<float, 3> u{10, 0, -5};
	/// auto x = u.Lenght() // 11.18034...
	/// \endcode
	///
	/// \tparam R the type in wwhich to calculate the result
	///
	/// \returns A scalar containing the distance
	////////////////////////////////////////////////////////////
	template <typename R = T>
	/** @cond */ __vector_inline /** @endcond */
		R
		Length() const noexcept
	{
		T x = T();
		vector_foreach_do(x += operator[](i))

		if constexpr (std::is_same_v<T, R>)
			return std::sqrt(x);
		else
			return std::sqrt(static_cast<R>(x));
	}

	////////////////////////////////////////////////////////////
	/// \brief Manhattan distance
	///
	/// Will caltulate the Manhattan distance distance (or norm 1) of the vector
	///
	/// \code{.cpp}
	/// Vector<float, 3> u{10, 0, -5};
	/// auto x = u.Manhattan(u) // 15
	/// \endcode
	///
	/// \returns A scalar containing the distance
	////////////////////////////////////////////////////////////
	/** @cond */ __vector_inline /** @endcond */
		T
		Manhattan() const noexcept
	{
		T r = T();
		vector_foreach_do(r += std::abs(operator[](i)));

		return r;
	}

	////////////////////////////////////////////////////////////
	/// \brief Squared euclidian distance
	///
	/// Will caltulate the squared euclidian distance (or suqared norm 2) of the vector
	///
	/// \code{.cpp}
	/// Vector<float, 3> u{10, 0, -5};
	/// auto x = u.LengtSqr(u) // 125
	/// \endcode
	///
	/// \returns A scalar containing the distance
	////////////////////////////////////////////////////////////
	/** @cond */ __vector_inline /** @endcond */
		T
		LengthSqr() const noexcept
	{
		T r = T();
		vector_foreach_do(r += operator[](i) * operator[](i));

		return r;
	}

	template <class R = T>
	/** @cond */ __vector_inline /** @endcond */
		void
		Normalize() noexcept
	{
		const R n = Length<R>();
		vector_foreach_do(operator[](i) /= n);
	}

	////////////////////////////////////////////////////////////
	/// \brief Dot product
	///
	/// Will return the dot product of u and v
	///
	/// \code{.cpp}
	/// Vector<float, 3> u{10, 0, -5};
	/// Vector<float, 3> v{0, 10, 1};
	/// auto x = Dot(u, v); // -5
	/// \endcode
	///
	/// \param u Left operand
	/// \param v Right operand
	/// \returns A scalar containing the dot product of u and v
	////////////////////////////////////////////////////////////
	/** @cond */ __vector_inline /** @endcond */
		friend T
		Dot(const Vector& u, const Vector& v) noexcept
	{
		T r = T();

		vector_foreach_do(r += u.d[i] * v.d[i]);

		return r;
	}

	////////////////////////////////////////////////////////////
	/// \brief Vector product
	///
	/// Will return the vector product of u and v
	/// \note u and v must be of dimension 3
	///
	///
	/// \code{.cpp}
	/// Vector<int, 3> i{1, 0, 0};
	/// Vector<int, 3> j{0, 1, 0};
	/// auto k = Product(i, j); // (0, 0, 1)
	/// \endcode
	///
	/// \param u Left operand
	/// \param v Right operand
	/// \returns A vector equal to the product (scalar/vectorial) of u and v
	////////////////////////////////////////////////////////////
	/** @cond */ __vector_inline /** @endcond */
		friend Vector<T, N, B, S>
		Product(const Vector& u, const Vector& v) noexcept
	{
		static_assert(N == 3);

		Vector<T, 3, B, S> r;

		r[0] = u[1] * v[2] - u[2] * v[1];
		r[1] = u[2] * v[0] - u[0] * v[2];
		r[2] = u[0] * v[1] - u[1] * v[0];

		return r;
	}

	////////////////////////////////////////////////////////////
	/// \brief Cross product
	///
	/// Will return the cross product of u and v
	/// \note u and v must be of dimension 2
	///
	///
	/// \code{.cpp}
	/// Vector<int, 2> i{1, 3};
	/// Vector<int, 2> j{4, 1};
	/// auto k = Cross(i, j); // -11
	/// \endcode
	///
	/// \param u Left operand
	/// \param v Right operand
	/// \returns A scalar equal to the cross product of u and v
	////////////////////////////////////////////////////////////
	/** @cond */ __vector_inline /** @endcond */
		friend T
		Cross(const Vector& u, const Vector& v) noexcept
	{
		static_assert(N == 2);

		return u[0] * v[1] - u[1] * v[0];
	}

	////////////////////////////////////////////////////////////
	/// \brief Intersection types
	///
	/// Contains informations about an intersection of two
	/// segments
	////////////////////////////////////////////////////////////
	enum class Intersection : std::uint8_t
	{
		NONE, ///< No intersection
		POINT, ///< Intersection is a point
		COLLINEAR, ///< The two segments are collinear
	};

	////////////////////////////////////////////////////////////
	/// \brief Segment intersection
	///
	/// Checks if two segments intersect. If so, calculates the
	/// intersection point.
	///
	/// \code{.cpp}
	/// auto&& [intersection, point] = Vector<int, 2>::Intersect<float>({{-1, 1}, {0,1}}, {{0, 0}, {0, 1}}); // {Intersection::POINT, (0, 1)}
	/// \endcode
	///
	/// \param s1 A pair containing the coordinates of both end
	/// points of the segment s1
	/// \param s2 A pair containing the coordinates of both end
	/// points of the segment s2
	/// \param eps A small number to account for rounding error
	/// (can be set to negative to check intersection of
	/// segments, excluding end point)
	/// \tparam R The scalar type in which to calculate the
	/// coordinates of the intersection point. Usually float or
	/// double.
	///
	/// \returns A pair containing:
	/// -# The type of intersection,
	/// -# (If there is an intersection) the coordinates of the intersection point
	///
	/// \see Intersection
	////////////////////////////////////////////////////////////
	template <typename R>
	/** @cond */ __vector_inline /** @endcond */
		std::pair<Intersection, Vector<R, N, B, S>> static Intersect(
			const std::pair<Vector, Vector>& s1,
			const std::pair<Vector, Vector>& s2,
			R eps = R(0.005)) noexcept
	{
		static_assert(N == 2);

		Vector<R, N, B, S> pt;
		auto Zero = [=]<typename K>(K x) { return std::abs(x) < eps; };

		Vector r = s1.second - s1.first;
		Vector s = s2.second - s2.first;
		T rxs = Cross(r, s);
		T qpxr = Cross(s2.first - s1.first, r);

		if (Zero(rxs))
			return { Intersection::COLLINEAR, pt };

		R t = static_cast<R>(Cross(s2.first - s1.first, s)) / static_cast<R>(rxs);

		R u = static_cast<R>(qpxr) / static_cast<R>(rxs);

		if ((R(0 - eps) <= t && t <= R(1 + eps)) && (R(0 - eps) <= u && u <= R(1 + eps)))
		{
			pt = static_cast<Vector<R, N, B, S>>(s1.first) +
				 static_cast<Vector<R, N, B, S>>(r) * t;

			return { Intersection::POINT, pt };
		}

		return { Intersection::NONE, pt };
	}

	////////////////////////////////////////////////////////////
	/// \brief Get element
	///
	/// \code{.cpp}
	/// Vector<int, 7> v{1, 7, 6, 0, -4, 1, 12};
	/// auto&& [x, y, z, w] = v;
	/// \endcode
	///
	/// \tparam M The index of the element to retreive
	/// \returns The element at index M
	////////////////////////////////////////////////////////////
	template <std::size_t M>
	/** @cond */ __vector_inline /** @endcond */
		decltype(auto)
		get() const noexcept
	{
		return operator[](M);
	}

	template <class... Args>
	Vector(Args... args)
	{
		static_assert(sizeof...(Args) == N);

		std::size_t i = 0;
		((operator[](i++) = args), ...);
	}
};
/** @cond */
namespace std
{
template <typename T, std::size_t N, template <class _T, std::size_t _N> class B, VectorSettings S>
struct tuple_size<Vector<T, N, B, S>> : std::integral_constant<std::size_t, N>
{};
}

namespace std
{
template <typename T, std::size_t N, template <class _T, std::size_t _N> class B, VectorSettings S, std::size_t M>
struct tuple_element<M, Vector<T, N, B, S>>
{
	using type = T;
};
}
/** @endcond */

#undef __expand
#undef vector_foreach_do
#undef __vector_inline

#endif // VECTOR_HPP
