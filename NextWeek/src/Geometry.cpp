#include <random>
#include <algorithm>
#include <iostream>

#include "Geometry.h"

// Ray

Ray::Ray(glm::vec3 pos, glm::vec3 dir, float time)
	:m_pos(pos), m_dir(dir), m_time(time)
{
}

glm::vec3 Ray::parameterize(float t) const
{
	return m_pos + t * m_dir;
}

/// Hitables

// AABB

AABB::AABB(glm::vec3 lower, glm::vec3 upper)
	:m_lower(lower), m_upper(upper)
{
}

bool AABB::hit(const Ray& r, float tmin, float tmax) const
{
	for (int i = 0; i < 3; i++)
	{
		float invD = 1.0f / r.dir()[i];
		float t0 = (min()[i] - r.pos()[i]) * invD;
		float t1 = (max()[i] - r.pos()[i]) * invD;
		if (invD < 0.0f)
		{
			std::swap(t0, t1);
		}
		tmin = t0 > tmin ? t0 : tmin;
		tmax = t1 < tmax ? t1 : tmax;
		if (tmax <= tmin)
			return false;
	}
	return true;
}

Hitable::Hitable(std::shared_ptr<Material> mat)
	:m_material(mat)
{
}

// BVH

BVHnode::BVHnode(std::vector<std::shared_ptr<Hitable>> list, float t0, float t1)
	:Hitable(nullptr)
{
	static std::default_random_engine e;
	static std::uniform_int_distribution<int> rand(0, 2);
	int axis = rand(e);

	auto compare = [axis, t0, t1](const std::shared_ptr<Hitable> & l, const std::shared_ptr<Hitable> & r) -> bool
	{
		AABB lBox, rBox;
		if (!l->boundingBox(t0, t1, lBox) || !r->boundingBox(t0, t1, rBox))
		{
			std::cerr << "No bounding box in BVHnode construction";
		}

		if (lBox.min()[axis] - rBox.min()[axis] < 0.0f)
			return true;
		else
			return false;
	};

	std::sort(list.begin(), list.end(), compare);

	if (list.size() == 1)
	{
		m_left = m_right = list[0];
	}
	else if (list.size() == 2)
	{
		m_left = list[0];
		m_right = list[1];
	}
	else
	{
		auto mid = list.begin() + list.size() / 2;
		m_left = std::make_shared<BVHnode>(std::vector<std::shared_ptr<Hitable>>(list.begin(), mid), t0, t1);
		m_right = std::make_shared<BVHnode>(std::vector<std::shared_ptr<Hitable>>(mid, list.end()), t0, t1);
	}
	AABB lBox, rBox;
	if (!m_left->boundingBox(t0, t1, lBox) || !m_right->boundingBox(t0, t1, rBox))
	{
		std::cerr << "No bounding box in BVHnode construction";
	}
	m_box = superBox(lBox, rBox);
}

bool BVHnode::hit(const Ray& r, float tmin, float tmax, HitRecord& rec) const
{
	if (m_box.hit(r, tmin, tmax))
	{
		HitRecord leftRec, rightRec;
		bool hitLeft = m_left->hit(r, tmin, tmax, leftRec);
		bool hitRight = m_right->hit(r, tmin, tmax, rightRec);

		if (hitLeft && hitRight)
		{
			if (leftRec.t < rightRec.t)
				rec = leftRec;
			else
				rec = rightRec;
			return true;
		}
		if (hitLeft)
		{
			rec = leftRec;
			return true;
		}
		if (hitRight)
		{
			rec = rightRec;
			return true;
		}
		return false;
	}
	return false;
}

bool BVHnode::boundingBox(float t0, float t1, AABB& box) const
{
	box = m_box;
	return true;
}


// Sphere

Sphere::Sphere(glm::vec3 center, float radius, std::shared_ptr<Material> mat)
	:Hitable(mat), m_center(center), m_radius(radius)
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
	rec.material = m_material;

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

bool Sphere::boundingBox(float t0, float t1, AABB& box) const
{
	box = AABB(m_center - glm::vec3(m_radius), m_center + glm::vec3(m_radius));
	return true;
}

// Moving Sphere

MovingSphere::MovingSphere(glm::vec3 ci, glm::vec3 cf, float ti, float tf, float radius, std::shared_ptr<Material> mat)
	:Hitable(mat), m_ci(ci), m_cf(cf), m_ti(ti), m_tf(tf), m_radius(radius)
{
}

bool MovingSphere::hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const
{
	rec.material = m_material;

	glm::vec3 pc = r.pos() - getCenter(r.time());
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
			rec.normal = (rec.p - getCenter(r.time())) / m_radius;
			return true;
		}
		temp = (-b + glm::sqrt(discriminant)) / (2.0f * a);
		if (temp > t_min && temp < t_max)
		{
			rec.t = temp;
			rec.p = r.parameterize(rec.t);
			rec.normal = (rec.p - getCenter(r.time())) / m_radius;
			return true;
		}
	}
	return false;
}

bool MovingSphere::boundingBox(float t0, float t1, AABB& box) const
{
	glm::vec3 radius(m_radius);
	AABB bi(m_ci - radius, m_ci + radius);
	AABB bf(m_cf - radius, m_cf + radius);
	box = superBox(bi, bf);
	return true;
}

// List

HitableList::HitableList(std::vector<std::shared_ptr<Hitable>> list)
	:Hitable(nullptr), m_list(list)
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

bool HitableList::boundingBox(float t0, float t1, AABB& box) const
{
	if (m_list.size() == 0)
		return false;
	AABB tempBox;
	bool firstTrue = m_list[0]->boundingBox(t0, t1, tempBox);
	if (!firstTrue)
		return false;
	else
		box = tempBox;
	for (auto& hitable : m_list)
	{
		if (hitable->boundingBox(t0, t1, tempBox))
			box = superBox(box, tempBox);
		else
			return false;
	}
	return true;
}

// Sampling

glm::vec3 randomInUnitSphere()
{
	static std::default_random_engine e;
	static std::uniform_real_distribution<float> u(-1.0f, 1.0f);

	glm::vec3 p;
	do
	{
		p = glm::vec3(u(e), u(e), u(e));
	} while (glm::dot(p, p) >= 1.0f);
	return p;
}

AABB superBox(const AABB& b1, const AABB& b2)
{
	glm::vec3 lower(fmin(b1.min().x, b2.min().x),
					fmin(b1.min().y, b2.min().y),
					fmin(b1.min().z, b2.min().z));

	glm::vec3 upper(fmax(b1.max().x, b2.max().x),
					fmax(b1.max().y, b2.max().y),
					fmax(b1.max().z, b2.max().z));

	return AABB(lower, upper);
}