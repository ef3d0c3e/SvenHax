#ifndef SDK_COLOR_HPP
#define SDK_COLOR_HPP

#include <array>
#include "../Util/Util.hpp"

class Color
{
	std::array<u8, 4> color;
public:
	// constructors
	Color() // std::array will auto-initialize with u8()
	{ }

	constexpr Color(u8 r, u8 g, u8 b):
		color{r, g, b, 0}
	{ }

	constexpr Color(u8 r, u8 g, u8 b, u8 a):
		color{r, g, b, a}
	{ }

	constexpr bool IsEmpty() const
	{
		return [&]<std::size_t... i>(std::index_sequence<i...>)
		{
			return ((color[i] == 0) && ...);
		}(std::make_index_sequence<4>{});
	}

	// set the color
	// r - red component (0-255)
	// g - green component (0-255)
	// b - blue component (0-255)
	// a - alpha component, controls transparency (0 - transparent, 255 - opaque);
	void SetColor(u8 r, u8 g, u8 b, u8 a = 0)
	{
		color[0] = r;
		color[1] = g;
		color[2] = b;
		color[3] = a;
	}

	void Reset()
	{
		[&]<std::size_t... i>(std::index_sequence<i...>)
		{
			((color[i] = 0), ...);
		}(std::make_index_sequence<4>{});
	}

	void SetRawABGR32(u32 colorHex32)
	{
		// ABGR32
		color[2] = (colorHex32) & 0xFF;
		color[1] = (colorHex32 >> 8) & 0xFF;
		color[0] = (colorHex32 >> 16) & 0xFF;
		color[3] = (colorHex32 >> 24) & 0xFF;
	}

	void SetRawRGBA32(u32 color32)
	{
		[&]<std::size_t... i>(std::index_sequence<i...>)
		{
			((color[i] = (color32 >> i*8) & 0xFF), ...);
		}(std::make_index_sequence<4>{});
	}

	int GetRawABGR32() const
	{
		return (color[3] << 24) | (color[0] << 16) | (color[1] << 8) | color[2];
	}

	int GetRawRGBA32() const
	{
		return [&]<std::size_t... i>(std::index_sequence<i...>)
		{
			return ((color[i] << i*8) | ...);
		}(std::make_index_sequence<4>{});
	}

	inline int r() const { return color[0]; }
	inline int g() const { return color[1]; }
	inline int b() const { return color[2]; }
	inline int a() const { return color[3]; }

	unsigned char &operator[](int index)
	{
		return color[index];
	}

	const unsigned char &operator[](int index) const
	{
		return color[index];
	}

	bool operator==(const Color &rhs) const
	{
		return (*((int *)this) == *((int *)&rhs));
	}

	bool operator!=(const Color &rhs) const
	{
		return !(operator==(rhs));
	}

	Color& operator=(const Color &rhs)
	{
		SetRawRGBA32(rhs.GetRawRGBA32());
		return *this;
	}
};

typedef Vector<u8, 3> Color24;

#endif // SDK_COLOR_HPP
