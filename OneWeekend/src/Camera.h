#pragma once
#include "glm.hpp"
#include "Geometry.h"

class Camera
{
public:
	Camera(glm::vec3 lookFrom, glm::vec3 lookAt, glm::vec3 up, float vfov, float aspect, float apature, float focusDist); // vfov is top to bottom in radians
	Ray getRay(float u, float v) const;
private:
	glm::vec3 m_origin;
	glm::vec3 m_lowerLeft;
	glm::vec3 m_horizontal;
	glm::vec3 m_vertical;
	glm::vec3 m_u, m_v, m_w;
	float m_lensRadius;

private:
	glm::vec3 randomInUnitDisk() const;
};