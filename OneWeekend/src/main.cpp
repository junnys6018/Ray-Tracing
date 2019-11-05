#include <iostream>
#include "stb_image.h"
#include "glm.hpp"

#include "ImageData.h"
#include "Geometry.h"

float hit_sphere(const glm::vec3& center, float radius, const Ray& r)
{
	glm::vec3 ac = r.pos() - center;
	float a = glm::dot(r.dir(), r.dir());
	float b = 2.0f * glm::dot(r.dir(), ac);
	float c = glm::dot(ac, ac) - radius * radius;

	float discriminant = b * b - 4.0f * a * c;
	if (discriminant < 0)
		return -1.0f;
	else
		return (-b - glm::sqrt(discriminant)) / (2.0f * a);
}

glm::vec3 color(const Ray& r)
{
	glm::vec3 center = glm::vec3(0.0f, 0.0f, -1.0f);
	float t = hit_sphere(center, 0.5f, r);
	if (t > 0)
	{
		glm::vec3 normal = r.parameterize(t) - center;
		return 0.5f * (glm::normalize(normal) + glm::vec3(1.0f));
	}
	t = 0.5f * (glm::normalize(r.dir()).y + 1.0f);
	return glm::mix(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.5f, 0.7f, 1.0f), t);
}


int main()
{
	std::cout << "Hello, world\n";

	ImageData image(200, 100);

	glm::vec3 lower_left(-2.0f, -1.0f, -1.0f);
	glm::vec3 horizontal(4.0f, 0.0f, 0.0f);
	glm::vec3 vertical(0.0f, 2.0f, 0.0f);
	glm::vec3 origin(0.0f, 0.0f, 0.0f);

	for (int y = 99; y >= 0; y--)
	{
		for (int x = 0; x < 200; x++)
		{
			float u = x / 200.0f;
			float v = y / 100.0f;

			Ray r(origin, lower_left + u * horizontal + v * vertical);
			glm::vec3 col = color(r);

			image << col;
		}
	}

	writefile("image.png", image);
	system("pause");
}