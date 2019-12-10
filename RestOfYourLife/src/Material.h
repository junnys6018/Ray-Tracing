#pragma once
#include "glm.hpp"
#include "Texture.h"
#include "Geometry.h"

class Material
{
public:
	virtual bool scatter(const Ray& r_in, const HitRecord& rec, glm::vec3& attenuation, Ray& scattered) const = 0;
	virtual glm::vec3 emitted(float u, float v, const glm::vec3& p) const { return glm::vec3(0.0f); }
};

class Lambertian : public Material
{
public:
	Lambertian(glm::vec3 albedo);
	bool scatter(const Ray& r_in, const HitRecord& rec, glm::vec3& attenuation, Ray& scattered) const override;
	glm::vec3 getAlbedo() const { return m_albedo; }

private:
	glm::vec3 m_albedo;
};

class Metal : public Material
{
public:
	Metal(glm::vec3 fresnel, float roughness);
	bool scatter(const Ray& r_in, const HitRecord& rec, glm::vec3& attenuation, Ray& scattered) const override;
	glm::vec3 getFresnel() const { return m_fresnel; }

private:
	glm::vec3 m_fresnel;
	float m_roughness;
};

class TexLambertian : public Material
{
public:
	TexLambertian(std::shared_ptr<Texture> albedo);
	bool scatter(const Ray& r_in, const HitRecord& rec, glm::vec3& attenuation, Ray& scattered) const override;
	std::shared_ptr<Texture> getAlbedo() const { return m_albedo; }

private:
	std::shared_ptr<Texture> m_albedo;
};

class TexMetal : public Material
{
public:
	TexMetal(std::shared_ptr<Texture> fresnel, float roughness);
	bool scatter(const Ray& r_in, const HitRecord& rec, glm::vec3& attenuation, Ray& scattered) const override;
	std::shared_ptr<Texture> getFresnel() const { return m_fresnel; }

private:
	std::shared_ptr<Texture> m_fresnel;
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

class DiffuseLight : public Material
{
public:
	DiffuseLight(std::shared_ptr<Texture> emit);
	bool scatter(const Ray& r_in, const HitRecord& rec, glm::vec3& attenuation, Ray& scattered) const override;
	virtual glm::vec3 emitted(float u, float v, const glm::vec3& p) const override;

private:
	std::shared_ptr<Texture> m_emit;
};

class Isotropic : public Material
{
public:
	Isotropic(std::shared_ptr<Texture> color);
	bool scatter(const Ray& r_in, const HitRecord& rec, glm::vec3& attenuation, Ray& scattered) const override;

private:
	std::shared_ptr<Texture> m_color;
};

float schlick(float cos, float refIndex);