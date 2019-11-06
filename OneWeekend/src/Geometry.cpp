#include "Geometry.h"

// Ray

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

// Sphere

Sphere::Sphere(glm::vec3 center, float radius)
	:m_center(center), m_radius(radius)
{
}

inline glm::vec3 Sphere::getCenter() const
{
	return m_center;
}

inline float Sphere::getRadius() const
{
	return m_radius;
}

bool Sphere::hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const
{
	glm::vec3 pc = r.pos() - m_center;
	float a = glm::dot(r.dir(), r.dir());
	float b = 2.0f * glm::dot(r.dir(), pc);
	float c = glm::dot(pc, pc) - m_radius * m_radius;

	float discriminant = b * b - 4.0f * a * c;
	if (discriminant >= 0.0f)
	{
		float temp = (-b - glm::sqrt(discriminant)) / (2.0f * a);
		if (temp > t_min && temp < t_max)
		{
			rec.t = temp;
			rec.p = r.parameterize(rec.t);
			rec.normal = (rec.p - m_center) / m_radius;
			return true;
		}
		temp = (-b + glm::sqrt(discriminant)) / (2.0f * a);
		if (temp > t_min && temp < t_max)
		{
			rec.t = temp;
			rec.p = r.parameterize(rec.t);
			rec.normal = (rec.p - m_center) / m_radius;
			return true;
		}
	}
	return false;
}

// List

HitableList::HitableList(std::vector<std::shared_ptr<Hitable>> list)
	:m_list(list)
{
}

bool HitableList::hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const
{
	HitRecord temp;
	bool hit = false;
	float closest = t_max;

	for (auto hitp : m_list)
	{
		if (hitp->hit(r, t_min, closest, temp))
		{
			hit = true;
			closest = temp.t;
			rec = temp;
		}
	}

	return hit;
}
