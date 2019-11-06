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

#define MULTI_SAMPLE

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

	float t = 0.5f * (glm::normalize(r.dir()).y + 1.0f);
	return glm::mix(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.5f, 0.7f, 1.0f), t);
}

std::shared_ptr<Hitable> generateScene()
{
	std::vector<std::shared_ptr<Hitable>> list;
	list.emplace_back(new Sphere(glm::vec3(0.0f, -1000.0f, 0.0f), 1000.0f, std::make_shared<Lambertian>(glm::vec3(0.5f, 0.5f, 0.5f))));

	std::default_random_engine e;
	std::uniform_real_distribution<float> u(0.0f, 1.0f);
	std::uniform_real_distribution<float> hu(0.5f, 1.0f);

	for (int a = -11; a < 11; a++)
	{
		for (int b = -11; b < 11; b++)
		{
			float chooseMat = u(e);
			glm::vec3 center(a + 0.9f * u(e), 0.2f, b + 0.9f * u(e));
			if (chooseMat < 0.8f)
			{
				list.emplace_back(new Sphere(center, 0.2f, std::make_shared<Lambertian>(glm::vec3(u(e) * u(e), u(e) * u(e), u(e) * u(e)))));
			}
			else if (chooseMat < 0.95f)
			{
				list.emplace_back(new Sphere(center, 0.2f, std::make_shared<Metal>(glm::vec3(hu(e), hu(e), hu(e)), 0.5f * u(e))));
			}
			else
			{
				list.emplace_back(new Sphere(center, 0.2f, std::make_shared<Dielectric>(1.5f)));
			}
		}
	}

	list.emplace_back(new Sphere(glm::vec3(0.0f, 1.0f, 0.0f), 1.0f, std::make_shared<Dielectric>(1.5f)));
	list.emplace_back(new Sphere(glm::vec3(-2.0f, 1.0f, 0.0f), 1.0f, std::make_shared<Lambertian>(glm::vec3(0.4f, 0.2f, 0.1f))));
	list.emplace_back(new Sphere(glm::vec3(2.0f, 1.0f, 0.0f), 1.0f, std::make_shared<Metal>(glm::vec3(0.7f, 0.6f, 0.5f), 0.0f)));

	return std::make_shared<HitableList>(list);
}

int main()
{
	const int nx = 1920, ny = 1080, ns = 50;
	ImageData image(nx, ny);

	//std::vector<std::shared_ptr<Hitable>> list;
	//list.push_back(std::shared_ptr<Hitable>(new Sphere(glm::vec3(0.0f, 0.0f, -1.0f), 0.5f, 
	//	std::shared_ptr<Material>(new Lambertian(glm::vec3(0.8f,0.3f,0.3f))))));
	//list.push_back(std::shared_ptr<Hitable>(new Sphere(glm::vec3(0.0f, -100.5f, -1.0f), 100.0f,
	//	std::shared_ptr<Material>(new Lambertian(glm::vec3(0.8f, 0.8f, 0.0f))))));
	//list.push_back(std::shared_ptr<Hitable>(new Sphere(glm::vec3(1.0f, 0.0f, -1.0f), 0.5f,
	//	std::shared_ptr<Material>(new Metal(glm::vec3(0.8f, 0.6f, 0.2f), 0.1f)))));
	//list.push_back(std::shared_ptr<Hitable>(new Sphere(glm::vec3(-1.0f, 0.0f, -1.0f), 0.5f,
	//	std::shared_ptr<Material>(new Dielectric(1.5f)))));
	//HitableList world(list);

	std::cout << "Generating Scene\n";
	auto world = generateScene();

	Camera cam(glm::vec3(13.0f, 2.0f, 3.0f), glm::vec3(0.0f), glm::vec3(0, 1, 0), glm::radians(20.0f), (float)nx / ny, 0.1f, 10.0f);
	std::default_random_engine e;
	std::uniform_real_distribution<float> uRand(0.0f, 0.999999999f);

	std::chrono::high_resolution_clock c;
	auto a = c.now();
	std::cout << "Rendering Scene\n";
	for (int y = ny - 1; y >= 0; y--)
	{
		for (int x = 0; x < nx; x++)
		{
#ifdef MULTI_SAMPLE
			glm::vec3 colour(0.0f, 0.0f, 0.0f);
			for (int s = 0; s < ns; s++)
			{
				float u = float(x + uRand(e)) / nx;
				float v = float(y + uRand(e)) / ny;
				Ray r = cam.getRay(u, v);
				colour += color(r, world.get());
			}
			colour /= ns;
			colour = glm::pow(colour, glm::vec3(0.5f));
			image << colour;
#else
			float u = (float)x / nx;
			float v = (float)y / ny;
			Ray r = cam.getRay(u, v);
			glm::vec3 col = color(r, &world);
			col = glm::pow(col, glm::vec3(0.5f));
			image << col;
#endif
		}
	}
	auto b = c.now();
	auto duration = b - a;
	std::cout << "Rendered in " << duration.count() * 1e-9 << " seconds\n";

#ifdef MULTI_SAMPLE
	writefile("multisample.png", image);
#else
	writefile("image.png", image);
#endif

	system("pause");
}