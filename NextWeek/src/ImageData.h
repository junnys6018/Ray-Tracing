#pragma once 
#include "glm.hpp"

struct Color
{
	Color() = default;
	Color(unsigned char, unsigned char, unsigned char);
	Color(float, float, float);
	unsigned char r, g, b;
};

class ImageData
{
public:
	ImageData(int width, int height);
	ImageData(const ImageData& other);
	ImageData& operator=(const ImageData& other);
	ImageData& operator<<(const Color& col);
	ImageData& operator<<(const glm::vec3& col);
	~ImageData();

	inline int getWidth() const { return m_width; }
	inline int getHeight() const { return m_height; }
	inline const Color* data() const { return m_data; }

	void setPixel(int x, int y, Color c);

private:
	int m_width, m_height;
	int m_index;
	Color* m_data;
};

int writefile(const char* filename, const ImageData& image);
