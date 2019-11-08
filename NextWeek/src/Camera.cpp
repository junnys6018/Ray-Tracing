#include "Camera.h"
#include <random>

Camera::Camera(glm::vec3 lookFrom, glm::vec3 lookAt, glm::vec3 up, float vfov,
	float aspect, float apature, float focusDist, float ti, float tf)
	:m_origin(lookFrom), m_lensRadius(apature / 2.0f), m_ti(ti), m_tf(tf)
{
	float halfHeight = glm::tan(vfov / 2.0f);
	float halfWidth = aspect * halfHeight;

	m_w = glm::normalize(lookFrom - lookAt);
	m_u = glm::cross(up, m_w);
	m_v = glm::cross(m_w, m_u);

	m_lowerLeft = m_origin - halfWidth * focusDist * m_u - halfHeight * focusDist * m_v - focusDist * m_w;
	m_horizontal = 2.0f * focusDist * halfWidth * m_u;
	m_vertical = 2.0f * focusDist * halfHeight * m_v;
}

Ray Camera::getRay(float u, float v) const
{
	static std::default_random_engine e;
	static std::uniform_real_distribution<float> rand(m_ti, m_tf);

	glm::vec3 rd = m_lensRadius * randomInUnitDisk();
	glm::vec3 offset = m_u * rd.x + m_v * rd.y;
	return Ray(m_origin + offset, m_lowerLeft + u * m_horizontal + v * m_vertical - m_origin - offset, rand(e));
}

glm::vec3 Camera::randomInUnitDisk() const
{
	static std::default_random_engine e;
	static std::uniform_real_distribution<float> u(-1.0f, 1.0f);
	glm::vec3 p;
	do
	{
		p = glm::vec3(u(e), u(e), 0.0f);
	} while (glm::dot(p, p) >= 1.0f);
	return p;
}
