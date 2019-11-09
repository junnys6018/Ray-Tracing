#include <iostream>
#include <limits>
#include <random>
#include <chrono>
#include <memory>
#include <thread>
#include <string>

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

void renderWorld(int seed, int nx, int ny, int ns, std::shared_ptr<Hitable> world, const Camera& cam, ImageData& image)
{
	std::default_random_engine e(seed);
	std::uniform_real_distribution<float> uRand(0.0f, 1.0f);

	for (int y = ny - 1; y >= 0; y--)
	{
		for (int x = 0; x < nx; x++)
		{
			glm::vec3 colour(0.0f, 0.0f, 0.0f);
			for (int s = 0; s < ns; s++)
			{
				float u = float(x + uRand(e)) / nx;
				float v = float(y + uRand(e)) / ny;
				Ray r = cam.getRay(u, v);
				colour += color(r, world.get());
			}
			colour /= ns;
			colour = glm::pow(colour, glm::vec3(1.0f / 2.2f));
			image << colour;
		}
	}
}

void renderWorldThreaded(int nx, int ny, int ns, int numThreads, std::shared_ptr<Hitable> world, const Camera& cam, ImageData& image)
{
	if (image.getHeight() != ny || image.getWidth() != nx)
	{
		std::cerr << "ImageData size does not match arguments!\n";
	}

	std::default_random_engine e(time(0));

	int samplesPerThread = (ns + numThreads - 1) / numThreads;
	std::vector<ImageData> images(numThreads, ImageData(nx, ny));
	std::vector<std::thread> threads;
	for (int i = 0; i < numThreads; i++)
	{
		threads.emplace_back(renderWorld, e(), nx, ny, samplesPerThread, world, cam, std::ref<ImageData>(images[i]));
	}

	for (auto& th : threads)
	{
		th.join();
	}

	//for (int i = 0; i < numThreads; i++)
	//{
	//	std::string s("image-" + std::to_string(i + 1) + ".png");
	//	writefile(s.c_str(), images[i]);
	//}

	for (int i = 0; i < nx * ny; i++)
	{
		int resultR = 0;
		int resultG = 0;
		int resultB = 0;
		for (int th = 0; th < numThreads; th++)
		{
			resultR += images[th].data()[i].r;
			resultG += images[th].data()[i].g;
			resultB += images[th].data()[i].b;
		}
		resultR /= numThreads;
		resultG /= numThreads;
		resultB /= numThreads;
		image << Color((unsigned char)resultR, (unsigned char)resultG, (unsigned char)resultB);
	}
}

int main()
{
	int nx = 1080, ny = 720, ns = 25;
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
	
	std::cout << "Rendering Scene ...\n";
	std::chrono::high_resolution_clock c;
	auto a = c.now();
	
	renderWorldThreaded(nx, ny, ns, 5, scene.world, scene.cam, image);

	auto b = c.now();
	auto duration = b - a;
	std::cout << "Rendered in " << duration.count() * 1e-9 << " seconds\n";

	writefile("image.png", image);

	system("pause");
	return 0;
}