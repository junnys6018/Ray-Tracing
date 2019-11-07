#pragma once
#include "glm.hpp"
#include "Geometry.h"

class Material
{
public:
	virtual bool scatter(const Ray& r_in, const HitRecord& rec, glm::vec3& attenuation, Ray& scattered) const = 0;
};

class Lambertian : public Material
{
public:
	Lambertian(glm::vec3 albedo);
	bool scatter(const Ray& r_in, const HitRecord& rec, glm::vec3& attenuation, Ray& scattered) const override;
	glm::vec3 getAlbedo() const;

private:
	glm::vec3 m_albedo;
};

class Metal : public Material
{
public:
	Metal(glm::vec3 fresnel, float roughness);
	bool scatter(const Ray& r_in, const HitRecord& rec, glm::vec3& attenuation, Ray& scattered) const override;
	glm::vec3 getFresnel() const;

private:
	glm::vec3 m_fresnel;
	float m_roughness;
};

class Dielectric : public Material
{
public:
	Dielectric(float ri);
	bool scatter(const Ray& r_in, const HitRecord& rec, glm::vec3& attenuation, Ray& scattered) const override;

private:
	float m_refIndex;
};

float schlick(float cos, float refIndex);