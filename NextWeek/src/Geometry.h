#pragma once
#include "glm.hpp"
#include <vector>
#include <memory>

class Ray
{
public:
	Ray() = default;
	Ray(glm::vec3 pos, glm::vec3 dir);

	glm::vec3 pos() const;
	glm::vec3 dir() const;
	glm::vec3 parameterize(float t) const;

private:
	glm::vec3 m_pos;
	glm::vec3 m_dir;
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
	Hitable(std::shared_ptr<Material> mat);
	virtual bool hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const = 0;

protected:
	std::shared_ptr<Material> m_material;
};

class Sphere : public Hitable
{
public:
	Sphere(glm::vec3 center, float radius, std::shared_ptr<Material> mat);

	inline glm::vec3 getCenter() const;
	inline float getRadius() const;

	bool hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const override;

private:
	glm::vec3 m_center;
	float m_radius;
};

class HitableList : public Hitable
{
public:
	HitableList(std::vector<std::shared_ptr<Hitable>> list);
	bool hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const override;

private:
	std::vector<std::shared_ptr<Hitable>> m_list;
};

glm::vec3 randomInUnitSphere();