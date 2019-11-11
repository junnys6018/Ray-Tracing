#pragma once
#include "glm.hpp"
#include <vector>
#include <memory>

#include "Texture.h"

class Ray
{
public:
	Ray() = default;
	Ray(glm::vec3 pos, glm::vec3 dir, float time);

	inline glm::vec3 pos() const { return m_pos; }
	inline glm::vec3 dir() const { return m_dir; }
	inline float time() const { return m_time; }
	glm::vec3 parameterize(float t) const;

private:
	glm::vec3 m_pos;
	glm::vec3 m_dir;
	float m_time;
};

class AABB
{
public:
	AABB() = default;
	AABB(glm::vec3 lower, glm::vec3 upper);

	inline glm::vec3 min() const { return m_lower; }
	inline glm::vec3 max() const { return m_upper; }

	bool hit(const Ray& r, float tmin, float tmax) const;
private:
	glm::vec3 m_lower, m_upper; // AABB represented by 3 intervals
};

class Material;

struct HitRecord
{
	float t;
	glm::vec3 p;
	glm::vec3 normal;
	std::shared_ptr<Material> material;
};

class Hitable
{
public:
	virtual bool hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const = 0;
	virtual bool boundingBox(float t0, float t1, AABB& box) const = 0;
};

class BVHnode : public Hitable
{
public:
	BVHnode() = default;
	BVHnode(std::vector<std::shared_ptr<Hitable>> list, float t0, float t1);

	bool hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const override;
	bool boundingBox(float t0, float t1, AABB& box) const override;

private:
	// Children
	std::shared_ptr<Hitable> m_left;
	std::shared_ptr<Hitable> m_right;

	AABB m_box; // Contains children
};

class Sphere : public Hitable
{
public:
	Sphere(glm::vec3 center, float radius, std::shared_ptr<Material> mat);

	inline glm::vec3 getCenter() const;
	inline float getRadius() const;

	bool hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const override;
	bool boundingBox(float t0, float t1, AABB& box) const override; // Function returns false is hitable has no bounding box (eg a plane)

private:
	glm::vec3 m_center;
	float m_radius;
	std::shared_ptr<Material> m_material;
};

class MovingSphere : public Hitable
{
public:
	MovingSphere(glm::vec3 ci, glm::vec3 cf, float ti, float tf, float radius, std::shared_ptr<Material> mat);

	inline glm::vec3 getCenter(float time) const
	{
		return m_ci + ((time - m_ti) / (m_tf - m_ti)) * (m_cf - m_ci);
	}
	inline float getRadius() const
	{
		return m_radius;
	}

	bool hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const override;
	bool boundingBox(float t0, float t1, AABB& box) const override;

private:
	glm::vec3 m_ci, m_cf;
	float m_ti, m_tf;
	float m_radius;
	std::shared_ptr<Material> m_material;
};

class HitableList : public Hitable
{
public:
	HitableList(std::vector<std::shared_ptr<Hitable>> list);
	bool hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const override;
	bool boundingBox(float t0, float t1, AABB& box) const override;

private:
	std::vector<std::shared_ptr<Hitable>> m_list;
};

class ConstantMedium : public Hitable
{
public:
	ConstantMedium(std::shared_ptr<Hitable> boundary, float density, std::shared_ptr<Texture> color);
	bool hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const override;
	bool boundingBox(float t0, float t1, AABB& box) const override;

private:
	std::shared_ptr<Hitable> m_boundary;
	float m_density;
	std::shared_ptr<Material> m_phaseFunction;
};

// Helpers
glm::vec3 randomInUnitSphere();
// Returns smallest AABB containing b1 and b2
AABB superBox(const AABB& b1, const AABB& b2);