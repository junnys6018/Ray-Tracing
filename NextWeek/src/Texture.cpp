#include "Texture.h"

ConstTexture::ConstTexture(glm::vec3 color)
	:m_color(color)
{
}

glm::vec3 ConstTexture::value(float u, float v, const glm::vec3& p) const
{
	return m_color;
}

CheckerTexture::CheckerTexture(std::shared_ptr<Texture> t0, std::shared_ptr<Texture> t1, float scale)
	:m_t0(t0), m_t1(t1), m_scale(scale)
{
}

glm::vec3 CheckerTexture::value(float u, float v, const glm::vec3& p) const
{
	float sign = sinf(m_scale * p.x) * sinf(m_scale * p.y) * sinf(m_scale * p.z);
	if (sign < 0.0f)
		return m_t0->value(u, v, p);
	else
		return m_t1->value(u, v, p);
}
