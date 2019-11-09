#include "ImageData.h"
#include "stb_image_write.h"
#include <cstring>
#include <cassert>

Color::Color(unsigned char _r, unsigned char _g, unsigned char _b)
	:r(_r), g(_g), b(_b)
{
}

Color::Color(float _r, float _g, float _b)
	: r(_r* 255.99), g(_g* 255.99), b(_b* 255.99)
{
	assert(_r >= 0.0f && _g >= 0.0f && _b >= 0.0f &&
		  _r <= 1.0f && _g <= 1.0f, && _b <= 1.0f);
}

ImageData::ImageData(int width, int height)
	:m_width(width), m_height(height), m_index(0), m_data(nullptr)
{
	m_data = new Color[m_width * m_height];
}


ImageData::ImageData(const ImageData& other)
	:m_width(other.m_width), m_height(other.m_height), m_index(other.m_index)
{
	m_data = new Color[m_width * m_height];

	memcpy(m_data, other.m_data, m_height * m_height * sizeof(Color));
}

ImageData& ImageData::operator=(const ImageData& other)
{
	m_width = other.m_width;
	m_height = other.m_height;
	m_index = other.m_index;

	Color* data = new Color[m_width * m_height];
	memcpy(data, other.m_data, m_height * m_height * sizeof(Color));
	delete[] m_data;
	m_data = data;

	return *this;
}

ImageData& ImageData::operator<<(const Color& col)
{
	assert(m_index < m_width * m_height);
	m_data[m_index++] = col;

	return *this;
}

ImageData& ImageData::operator<<(const glm::vec3& col)
{
	Color c(col.r, col.g, col.b);

	return *this << c;
}

ImageData::~ImageData()
{
	delete[] m_data;
}

void ImageData::setPixel(int x, int y, Color c)
{
	assert(x >= 0 && y >= 0 && x < m_width && y < m_height);
	int index = y * m_width + x;
	m_data[index] = c;
}

int writefile(const char* filename, const ImageData& image)
{
	return stbi_write_png(filename, image.getWidth(), image.getHeight(), 3, image.data(), image.getWidth() * sizeof(Color));
}


