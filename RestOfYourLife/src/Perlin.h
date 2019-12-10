#pragma once
#include "glm.hpp"

class Perlin
{
public:
	float noise(const glm::vec3& p) const;
	float turb(const glm::vec3& p, int depth = 7) const;

private:
	static glm::vec3* s_ranVec3;
	static int* s_permX;
	static int* s_permY;
	static int* s_permZ;
};

glm::vec3* perlinGenerate();
void permute(int* p, int n);
int* perlinGeneratePerm();