#include "Camera.h"

Camera::Camera()
	:m_lowerLeft(-2.0f, -1.0f, -1.0f), m_origin(0.0f, 0.0f, 0.0f), m_horizontal(4.0f, 0.0f, 0.0f), m_vertical(0.0f, 2.0f, 0.0f)
{
}

Ray Camera::getRay(float u, float v) const
{
	return Ray(m_origin, m_lowerLeft + u * m_horizontal + v * m_vertical - m_origin);
}
