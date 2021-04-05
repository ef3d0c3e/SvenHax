#ifndef UI_IMAGE_HPP
#define UI_IMAGE_HPP

#include "../Engine/Primitives/Defs.hpp"
#include "../Engine/Primitives/Vector.hpp"
#include "../Util/Util.hpp"
#include <GL/gl.h>

class Image
{
	const unsigned char* m_data;
	std::size_t m_size;

	// GL data
	GLuint m_texture;
	Vector<int, 2> m_dim;

public:
	Image(const unsigned char* image, std::size_t size)
	{
		m_data = image;
		m_size = size;
	}
	~Image()
	{ }

	Vector<int, 2> GetSize() const
	{
		return m_dim;
	}

	const GLuint GetTexture() const
	{
		return m_texture;
	}

	void Load();
};

#endif // UI_IMAGE_HPP
