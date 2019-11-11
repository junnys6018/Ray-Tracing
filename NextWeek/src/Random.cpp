#include "Random.h"

#include <random>

static std::default_random_engine e;
static std::uniform_real_distribution<float> u(-1.0f, 1.0f);

glm::vec3 randomInUnitDisk()
{
	glm::vec3 p;
	do
	{
		p = glm::vec3(u(e), u(e), 0.0f);
	} while (glm::dot(p, p) >= 1.0f);
	return p;
}

glm::vec3 randomInUnitSphere()
{
	glm::vec3 p;
	do
	{
		p = glm::vec3(u(e), u(e), u(e));
	} while (glm::dot(p, p) >= 1.0f);
	return p;
}

float myRand()
{
	return u(e);
}