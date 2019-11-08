#include "SceneFactory.h"

#include <random>

#include "Material.h"

std::vector<std::function<Scene(float)>> getSceneFactories()
{
	return { scene1,scene2 };
}

Scene scene1(float aspect)
{
	std::vector<std::shared_ptr<Hitable>> list;
	list.push_back(std::shared_ptr<Hitable>(new Sphere(glm::vec3(0.0f, -100.5f, -1.0f), 100.0f,
		std::shared_ptr<Material>(new Lambertian(glm::vec3(0.8f, 0.8f, 0.0f))))));
	list.push_back(std::shared_ptr<Hitable>(new MovingSphere(glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, -1.0f),
		0.0f, 1.5f, 0.5f, std::shared_ptr<Material>(new Lambertian(glm::vec3(0.8f, 0.3f, 0.3f))))));
	list.push_back(std::shared_ptr<Hitable>(new Sphere(glm::vec3(1.0f, 0.0f, -1.0f), 0.5f,
		std::shared_ptr<Material>(new Metal(glm::vec3(0.8f, 0.8f, 0.8f), 0.1f)))));
	list.push_back(std::shared_ptr<Hitable>(new Sphere(glm::vec3(-1.0f, 0.0f, -1.0f), 0.5f,
		std::shared_ptr<Material>(new Dielectric(1.5f)))));

	glm::vec3 lookfrom(0.0f, 1.0f, 3.0f);
	glm::vec3 lookat(0.0f, 0.0f, -1.0f);
	float apature = 0.0f;
	Camera cam(lookfrom, lookat, glm::vec3(0, 1, 0), glm::radians(50.0f), aspect, apature, 10.0f, 0.5f, 0.8f);

	return { std::make_shared<BVHnode>(list, 0.0f, std::numeric_limits<float>::max()),cam };
}

Scene scene2(float aspect)
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
			if (chooseMat < 0.3f)
			{
				list.emplace_back(new MovingSphere(center, center + glm::vec3(0.0f, 0.2f, 0.0f), 0.0f, 1.0f, 0.2f,
					std::make_shared<Lambertian>(glm::vec3(u(e) * u(e), u(e) * u(e), u(e) * u(e)))));
			}
			else if (chooseMat < 0.8f)
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

	list.emplace_back(new Sphere(glm::vec3(0.0f, 1.5f, 0.0f), 1.5f, std::make_shared<Dielectric>(1.5f)));
	list.emplace_back(new Sphere(glm::vec3(-3.2f, 1.5f, 0.0f), 1.5f, std::make_shared<Lambertian>(glm::vec3(0.4f, 0.2f, 0.1f))));
	list.emplace_back(new Sphere(glm::vec3(3.2f, 1.5f, 0.0f), 1.5f, std::make_shared<Metal>(glm::vec3(0.7f, 0.6f, 0.5f), 0.0f)));

	glm::vec3 lookfrom(0.0f, 2.0f, 13.0f);
	glm::vec3 lookat(0.0f, 0.0f, -1.0f);
	float apature = 0.2f;
	Camera cam(lookfrom, lookat, glm::vec3(0, 1, 0), glm::radians(50.0f), aspect, apature, 10.0f, 0.5f, 0.8f);

	return { std::make_shared<BVHnode>(list, 0.0f, std::numeric_limits<float>::max()), cam };
}