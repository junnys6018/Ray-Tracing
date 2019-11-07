#pragma once 
#include "glm.hpp"

struct Color
{
	Color() = default;
	Color(char, char, char);
	Color(float, float, float);
	char r, g, b;
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

	inline int getWidth() const;
	inline int getHeight() const;
	inline const Color* data() const;

	void setPixel(int x, int y, Color c);

private:
	int m_width, m_height;
	int m_index;
	Color* m_data;
};

int writefile(const char* filename, const ImageData& image);
