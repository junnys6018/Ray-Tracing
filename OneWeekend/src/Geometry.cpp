#include "Geometry.h"

Ray::Ray(glm::vec3 pos, glm::vec3 dir)
	:m_pos(pos), m_dir(dir)
{
}

glm::vec3 Ray::pos() const
{
	return m_pos;
}

glm::vec3 Ray::dir() const
{
	return m_dir;
}

glm::vec3 Ray::parameterize(float t) const
{
	return m_pos + t * m_dir;
}
