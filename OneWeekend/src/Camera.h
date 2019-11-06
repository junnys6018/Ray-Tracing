#pragma once
#include "glm.hpp"
#include "Geometry.h"

class Camera
{
public:
	Camera();
	Ray getRay(float u, float v) const;
private:
	glm::vec3 m_origin;
	glm::vec3 m_lowerLeft;
	glm::vec3 m_horizontal;
	glm::vec3 m_vertical;
};