#include <iostream>
#include <limits>
#include "stb_image.h"
#include "glm.hpp"

#include "ImageData.h"
#include "Geometry.h"



glm::vec3 color(const Ray& r, Hitable* world)
{
	HitRecord rec;
	if (world->hit(r, 0, std::numeric_limits<float>::max(), rec))
	{
		return 0.5f* (rec.normal + glm::vec3(1.0f));
	}

	float t = 0.5f * (glm::normalize(r.dir()).y + 1.0f);
	return glm::mix(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.5f, 0.7f, 1.0f), t);
}


int main()
{
	const int nx = 1080, ny = nx / 2;
	ImageData image(nx, ny);

	glm::vec3 lower_left(-2.0f, -1.0f, -1.0f);
	glm::vec3 horizontal(4.0f, 0.0f, 0.0f);
	glm::vec3 vertical(0.0f, 2.0f, 0.0f);
	glm::vec3 origin(0.0f, 0.0f, 0.0f);

	std::vector<std::shared_ptr<Hitable>> list;
	list.push_back(std::shared_ptr<Hitable>(new Sphere(glm::vec3(0.0f, 0.0f, -1.0f), 0.5f)));
	list.push_back(std::shared_ptr<Hitable>(new Sphere(glm::vec3(0.0f, -100.5f, -1.0f), 100.0f)));
	HitableList world(list);

	for (int y = ny - 1; y >= 0; y--)
	{
		for (int x = 0; x < nx; x++)
		{
			float u = (float)x / nx;
			float v = (float)y / ny;

			Ray r(origin, lower_left + u * horizontal + v * vertical);
			glm::vec3 col = color(r, &world);

			image << col;
		}
	}

	writefile("image.png", image);
	system("pause");
}