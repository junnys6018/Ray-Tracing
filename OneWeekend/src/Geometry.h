#pragma once
#include "glm.hpp"

class Ray
{
public:
	Ray(glm::vec3 pos, glm::vec3 dir);

	glm::vec3 pos() const;
	glm::vec3 dir() const;
	glm::vec3 parameterize(float t) const;

private:
	glm::vec3 m_pos;
	glm::vec3 m_dir;
};