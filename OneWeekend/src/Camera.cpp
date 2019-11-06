#include "Camera.h"

Camera::Camera(glm::vec3 lookFrom, glm::vec3 lookAt, glm::vec3 up, float vfov, float aspect)
{
	float halfHeight = glm::tan(vfov / 2.0f);
	float halfWidth = aspect * halfHeight;

	glm::vec3 w = glm::normalize(lookFrom - lookAt);
	glm::vec3 u = glm::cross(up, w);
	glm::vec3 v = glm::cross(w, u);

	m_origin = lookFrom;
	m_lowerLeft = m_origin - halfWidth * u - halfHeight * v - w;
	m_horizontal = 2.0f * halfWidth * u;
	m_vertical = 2.0f * halfHeight * v;
}

Ray Camera::getRay(float u, float v) const
{
	return Ray(m_origin, m_lowerLeft + u * m_horizontal + v * m_vertical - m_origin);
}
