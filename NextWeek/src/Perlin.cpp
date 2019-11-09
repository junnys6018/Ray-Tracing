#include "Perlin.h"
#include <random>

static std::default_random_engine e;
static std::uniform_real_distribution<float> u(0.0f, 1.0f);
static std::uniform_real_distribution<float> uu(-1.0f, 1.0f);

glm::vec3* Perlin::s_ranVec3 = perlinGenerate();
int* Perlin::s_permX = perlinGeneratePerm();
int* Perlin::s_permY = perlinGeneratePerm();
int* Perlin::s_permZ = perlinGeneratePerm();

inline float perlinInterpolate(glm::vec3 c[2][2][2], float u, float v, float w)
{
	glm::vec3 weight(u, v, w);
	// Transform coords for better interpolation see: https://en.wikipedia.org/wiki/Cubic_Hermite_spline
	u = u * u * (3.0f - 2.0f * u);
	v = v * v * (3.0f - 2.0f * v);
	w = w * w * (3.0f - 2.0f * w);
	float total = 0.0f;
	for(int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			for (int k = 0; k < 2; k++)
			{
				total += (i * u + (1 - i) * (1 - u)) *
						 (j * v + (1 - j) * (1 - v)) *
						 (k * w + (1 - k) * (1 - w)) * glm::dot(c[i][j][k], weight - glm::vec3(i, j, k));
			}
		}
	}
	return total;
}


float Perlin::noise(const glm::vec3& p) const
{
	int i = glm::floor(p.x);
	int j = glm::floor(p.y);
	int k = glm::floor(p.z);

	glm::vec3 c[2][2][2];
	// Sample a 2x2x2 cube and interpolate
	for (int di = 0; di < 2; di++)
	{
		for (int dj = 0; dj < 2; dj++)
		{
			for (int dk = 0; dk < 2; dk++)
			{
				c[di][dj][dk] = s_ranVec3[s_permX[(i + di) & 255] ^ s_permY[(j + dj) & 255] ^ s_permZ[(k + dk) & 255]];
			}
		}
	}

	float u = p.x - glm::floor(p.x);
	float v = p.y - glm::floor(p.y);
	float w = p.z - glm::floor(p.z);
	return perlinInterpolate(c, u, v, w);
}

float Perlin::turb(const glm::vec3& p, int depth) const
{
	float total = 0.0f;
	glm::vec3 _p = p;
	float weight = 1.0f;
	for (int i = 0; i < depth; i++)
	{
		total += weight * noise(_p);
		weight *= 0.5f;
		_p *= 2.0f;
	}
	return glm::abs(total);
}

glm::vec3* perlinGenerate()
{
	glm::vec3* p = new glm::vec3[256];

	for (int i = 0; i < 256; i++)
	{
		p[i] = glm::normalize(glm::vec3(uu(e), uu(e), uu(e)));
	}
	return p;
}

void permute(int* p, int n)
{
	for (int i = n - 1; i > 0; i--)
	{
		int target = int(u(e) * i);
		int temp = p[i];
		p[i] = p[target];
		p[target] = temp;
	}
}

int* perlinGeneratePerm()
{
	int* p = new int[256];
	for (int i = 0; i < 250; i++)
	{
		p[i] = i;
	}
	permute(p, 256);
	return p;
}
