#include <iostream>
#include <limits>
#include <random>
#include <chrono>
#include <memory>

#include "stb_image.h"
#include "glm.hpp"

#include "ImageData.h"
#include "Geometry.h"
#include "Camera.h"
#include "Material.h"
#include "SceneFactory.h"

glm::vec3 color(const Ray& r, Hitable* world, int depth = 0)
{
	HitRecord rec;
	if (world->hit(r, 0.001f, std::numeric_limits<float>::max(), rec))
	{
		Ray scattered;
		glm::vec3 attenuation;
		if (depth < 50 && rec.material->scatter(r, rec, attenuation, scattered))
		{
			return attenuation * color(scattered, world, depth + 1);
		}
		else
			return glm::vec3(0.0f);
	}

	// Else return sky gradient
	float t = 0.5f * (glm::normalize(r.dir()).y + 1.0f);
	return glm::mix(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.5f, 0.7f, 1.0f), t);
}

int main()
{
	int nx = 1080, ny = 720, ns = 20;
	ImageData image(nx, ny);

	auto factory = getSceneFactories();
	std::cout << "Enter a number from 1 - " << factory.size() << " to select scene: ";
	size_t i;
	std::cin >> i;
	if (i <= 0 || i > factory.size())
	{
		std::cerr << "Not in Range\n";
		system("pause");
		return 1;
	}
	
	Scene scene = factory[i - 1]((float)nx / ny);

	std::default_random_engine e;
	std::uniform_real_distribution<float> uRand(0.0f, 1.0f);

	std::chrono::high_resolution_clock c;
	auto a = c.now();
	std::cout << "Rendering Scene ...\n";
	for (int y = ny - 1; y >= 0; y--)
	{
		for (int x = 0; x < nx; x++)
		{
			glm::vec3 colour(0.0f, 0.0f, 0.0f);
			for (int s = 0; s < ns; s++)
			{
				float u = float(x + uRand(e)) / nx;
				float v = float(y + uRand(e)) / ny;
				Ray r = scene.cam.getRay(u, v);
				colour += color(r, scene.world.get());
			}
			colour /= ns;
			colour = glm::pow(colour, glm::vec3(0.5f));
			image << colour;
		}
	}
	auto b = c.now();
	auto duration = b - a;
	std::cout << "Rendered in " << duration.count() * 1e-9 << " seconds\n";

	writefile("image.png", image);

	system("pause");
	return 0;
}