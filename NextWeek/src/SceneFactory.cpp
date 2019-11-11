#include "SceneFactory.h"

#include <random>

#include "Material.h"
#include "Texture.h"

std::vector<std::function<Scene(float)>> getSceneFactories()
{
	return { scene1,scene2, scene3,scene4,scene5,scene6 };
}

static auto daySkybox = [](glm::vec3 dir) -> glm::vec3
{
	float t = 0.5f * (glm::normalize(dir).y + 1.0f);
	return glm::mix(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.5f, 0.7f, 1.0f), t);
};

static auto nightSkybox = [](glm::vec3 dir) -> glm::vec3
{
	return glm::vec3(0.0f);
};

Scene scene1(float aspect)
{
	std::vector<std::shared_ptr<Hitable>> list;
	list.emplace_back(new Sphere(glm::vec3(0.0f, -100.5f, -1.0f), 100.0f, std::make_shared<Lambertian>(glm::vec3(0.8f, 0.8f, 0.0f))));
	list.emplace_back(new MovingSphere(glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, -1.0f),
		0.0f, 1.5f, 0.5f, std::make_shared<Lambertian>(glm::vec3(0.8f, 0.3f, 0.3f))));
	list.emplace_back(new Sphere(glm::vec3(1.0f, 0.0f, -1.0f), 0.5f, std::make_shared<Metal>(glm::vec3(0.8f, 0.8f, 0.8f), 0.1f)));
	list.emplace_back(new Sphere(glm::vec3(-1.0f, 0.0f, -1.0f), 0.5f, std::make_shared<Dielectric>(1.5f)));

	glm::vec3 lookfrom(0.0f, 1.0f, 3.0f);
	glm::vec3 lookat(0.0f, 0.0f, -1.0f);
	float apature = 0.0f;
	Camera cam(lookfrom, lookat, glm::vec3(0, 1, 0), glm::radians(50.0f), aspect, apature, 10.0f, 0.5f, 0.8f);

	return { std::make_shared<BVHnode>(list, 0.5f, 0.8f), cam, daySkybox };
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
	float apature = 0.1f;
	Camera cam(lookfrom, lookat, glm::vec3(0, 1, 0), glm::radians(50.0f), aspect, apature, 10.0f, 0.5f, 0.8f);

	return { std::make_shared<BVHnode>(list, 0.0f, std::numeric_limits<float>::max()), cam,daySkybox };
}

Scene scene3(float aspect)
{
	std::shared_ptr<CheckerTexture> checker = std::make_shared<CheckerTexture>(std::make_shared<ConstTexture>(glm::vec3(0.2f, 0.3f, 0.1f)),
		std::make_shared<ConstTexture>(glm::vec3(0.9f, 0.9f, 0.9f)));

	std::vector<std::shared_ptr<Hitable>> list;
	list.emplace_back(new Sphere(glm::vec3(0.0f, 10.0f, 0.0f), 10.0f, std::make_shared<TexLambertian>(checker)));
	list.emplace_back(new Sphere(glm::vec3(0.0f, -10.0f, 0.0f), 10.0f, std::make_shared<TexLambertian>(checker)));

	glm::vec3 lookfrom(13.0f, 2.0f, 3.0f);
	glm::vec3 lookat(0.0f);
	float apature = 0.0f;
	Camera cam(lookfrom, lookat, glm::vec3(0, 1, 0), glm::radians(50.0f), aspect, apature, 10.0f, 0.5f, 0.8f);

	return { std::make_shared<BVHnode>(list, 0.0f, std::numeric_limits<float>::max()), cam, daySkybox };
}

Scene scene4(float aspect)
{
	std::shared_ptr<MarbleTexture> marble = std::make_shared<MarbleTexture>(4.0f);
	std::shared_ptr<CheckerTexture> checker = std::make_shared<CheckerTexture>(std::make_shared<ConstTexture>(glm::vec3(0.2f, 0.3f, 0.1f)),
		std::make_shared<ConstTexture>(glm::vec3(0.9f, 0.9f, 0.9f)));
	std::vector<std::shared_ptr<Hitable>> list;

	list.emplace_back(new Sphere(glm::vec3(0.0f, -1000.0f, 0.0f), 1000.0f, std::make_shared<TexLambertian>(checker)));
	list.emplace_back(new Sphere(glm::vec3(0.0f, 2.0f, 0.0f), 2.0f, std::make_shared<TexLambertian>(marble)));
	list.emplace_back(new Sphere(glm::vec3(2.5f, 0.6f, 0.0f), 0.6f, std::make_shared<Dielectric>(1.5f)));

	glm::vec3 lookfrom(13.0f, 2.0f, 0.0f);
	glm::vec3 lookat(0.0f);
	float apature = 0.0f;
	Camera cam(lookfrom, lookat, glm::vec3(0, 1, 0), glm::radians(20.0f), aspect, apature, 10.0f, 0.5f, 0.8f);

	return { std::make_shared<BVHnode>(list, 0.0f, std::numeric_limits<float>::max()), cam, daySkybox };
}

Scene scene5(float aspect)
{
	std::shared_ptr<MarbleTexture> marble = std::make_shared<MarbleTexture>(4.0f);
	std::shared_ptr<CheckerTexture> checker = std::make_shared<CheckerTexture>(std::make_shared<ConstTexture>(glm::vec3(0.2f, 0.3f, 0.1f)),
		std::make_shared<ConstTexture>(glm::vec3(0.9f, 0.9f, 0.9f)));
	std::shared_ptr<ConstTexture> light = std::make_shared<ConstTexture>(glm::vec3(4.0f));

	std::vector<std::shared_ptr<Hitable>> list;

	list.emplace_back(new Sphere(glm::vec3(0.0f, -1000.0f, 0.0f), 1000.0f, std::make_shared<TexLambertian>(checker)));
	list.emplace_back(new Sphere(glm::vec3(0.0f, 2.0f, 0.0f), 2.0f, std::make_shared<TexLambertian>(marble)));
	list.emplace_back(new Sphere(glm::vec3(3.0f, 1.0, 2.0f), 1.0f, std::make_shared<Metal>(glm::vec3(0.7f, 0.7f, 0.7f), 0.4f)));
	list.emplace_back(new Sphere(glm::vec3(3.0f, 1.0, -2.0f), 1.0f, std::make_shared<Metal>(glm::vec3(0.7f, 0.5f, 0.5f), 0.05f)));
	list.emplace_back(new Sphere(glm::vec3(4.0, 3.0f, 0.0f), 0.4f, std::make_shared<DiffuseLight>(light)));

	glm::vec3 lookfrom(13.0f, 2.0f, 0.0f);
	glm::vec3 lookat(0.0f);
	float apature = 0.0f;
	Camera cam(lookfrom, lookat, glm::vec3(0, 1, 0), glm::radians(40.0f), aspect, apature, 10.0f, 0.5f, 0.8f);

	return { std::make_shared<BVHnode>(list, 0.0f, std::numeric_limits<float>::max()), cam, nightSkybox };
}

Scene scene6(float aspect)
{
	std::vector<std::shared_ptr<Hitable>> list;
	list.emplace_back(new Sphere(glm::vec3(0.0f, -1000.0f, -1.0f), 1000.0f, std::make_shared<Lambertian>(glm::vec3(0.8f, 0.8f, 0.0f))));
	list.emplace_back(new Sphere(glm::vec3(0.0f, 0.0f, -540.0f), 500.0f, std::make_shared<TexLambertian>(std::make_shared<MarbleTexture>(2.0f))));
	list.emplace_back(new Sphere(glm::vec3(-540.0f, 0.0f, 0.0f), 500.0f, std::make_shared<Metal>(glm::vec3(0.8f, 0.8f, 0.8f), 0.01f)));
	list.emplace_back(new MovingSphere(glm::vec3(0.0f, 0.5f, -1.0f), glm::vec3(0.0f, 1.5f, -1.0f),
		0.0f, 1.5f, 0.5f, std::make_shared<Lambertian>(glm::vec3(0.8f, 0.3f, 0.3f))));
	list.emplace_back(new Sphere(glm::vec3(1.0f, 0.5f, -1.0f), 0.5f, std::make_shared<Metal>(glm::vec3(0.8f, 0.8f, 0.8f), 0.01f)));
	list.emplace_back(new Sphere(glm::vec3(-1.5f, 0.5f, -1.5f), 0.5f, std::make_shared<Dielectric>(1.5f)));

	glm::vec3 lowerCorner = glm::vec3(-2.4f, 0.2f, -1.5f);
	glm::vec3 upperCorner = lowerCorner + glm::vec3(2.0f);
	float radius = (upperCorner.x - lowerCorner.x) / 10.0f;
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			for (int k = 0; k < 5; k++)
			{
				auto a = std::shared_ptr<Hitable>(new Sphere(lowerCorner + glm::vec3(i, j, k) * (upperCorner - lowerCorner) / glm::vec3(5.0f)
					, radius, nullptr));
				list.emplace_back(new ConstantMedium(a, 10.0f, std::make_shared<ConstTexture>(glm::vec3(1.0f))));
			}
		}
	}

	glm::vec3 lookfrom(3.0f, 1.5f, 3.0f);
	glm::vec3 lookat(0.0f, 0.5f, -1.0f);
	float apature = 0.0f;
	Camera cam(lookfrom, lookat, glm::vec3(0, 1, 0), glm::radians(50.0f), aspect, apature, 10.0f, 0.5f, 0.8f);

	return { std::make_shared<BVHnode>(list, 0.5f, 0.8f), cam, daySkybox };
}
