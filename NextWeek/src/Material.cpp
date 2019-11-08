#include "Material.h"

#include <random>

Lambertian::Lambertian(glm::vec3 albedo)
	:m_albedo(albedo)
{
}

bool Lambertian::scatter(const Ray& r_in, const HitRecord& rec, glm::vec3& attenuation, Ray& scattered) const
{
	glm::vec3 target = rec.p + rec.normal + randomInUnitSphere();
	scattered = Ray(rec.p, target - rec.p, r_in.time());
	attenuation = m_albedo;
	return true;
}

Metal::Metal(glm::vec3 fresnel, float roughness)
	:m_fresnel(fresnel), m_roughness(roughness)
{
	if (m_roughness > 1.0f)
		m_roughness = 1.0f;
}

bool Metal::scatter(const Ray& r_in, const HitRecord& rec, glm::vec3& attenuation, Ray& scattered) const
{
	glm::vec3 reflected = glm::normalize(glm::reflect(r_in.dir(), rec.normal)) + m_roughness * randomInUnitSphere();
	scattered = Ray(rec.p, reflected, r_in.time());
	attenuation = m_fresnel;
	return glm::dot(scattered.dir(), rec.normal) > 0;
}

TexLambertian::TexLambertian(std::shared_ptr<Texture> albedo)
	:m_albedo(albedo)
{
}

bool TexLambertian::scatter(const Ray& r_in, const HitRecord& rec, glm::vec3& attenuation, Ray& scattered) const
{
	glm::vec3 target = rec.p + rec.normal + randomInUnitSphere();
	scattered = Ray(rec.p, target - rec.p, r_in.time());
	attenuation = m_albedo->value(0.0f, 0.0f, rec.p);
	return true;
}


TexMetal::TexMetal(std::shared_ptr<Texture> fresnel, float roughness)
	:m_fresnel(fresnel), m_roughness(roughness)
{
}

bool TexMetal::scatter(const Ray& r_in, const HitRecord& rec, glm::vec3& attenuation, Ray& scattered) const
{
	glm::vec3 reflected = glm::normalize(glm::reflect(r_in.dir(), rec.normal)) + m_roughness * randomInUnitSphere();
	scattered = Ray(rec.p, reflected, r_in.time());
	attenuation = m_fresnel->value(0.0f, 0.0f, rec.p);
	return glm::dot(scattered.dir(), rec.normal) > 0;
}

Dielectric::Dielectric(float ri)
	:m_refIndex(ri)
{
}

bool Dielectric::scatter(const Ray& r_in, const HitRecord& rec, glm::vec3& attenuation, Ray& scattered) const
{
	attenuation = glm::vec3(1.0f, 1.0f, 1.0f);

	glm::vec3 reflected = glm::normalize(glm::reflect(r_in.dir(), rec.normal));
	float eta, cos;
	glm::vec3 out_normal;
	if (glm::dot(r_in.dir(), rec.normal) > 0)
	{
		eta = m_refIndex;
		cos = glm::dot(r_in.dir(), rec.normal) / glm::length(r_in.dir());
		out_normal = -rec.normal;
	}
	else
	{
		eta = 1.0f / m_refIndex;
		cos = -glm::dot(r_in.dir(), rec.normal) / glm::length(r_in.dir());
		out_normal = rec.normal;
	}
	float reflect_prob;
	glm::vec3 refracted = glm::refract(glm::normalize(r_in.dir()), out_normal, eta);
	if (glm::length(refracted) < 0.01f)
	{
		reflect_prob = 1.0f;
	}
	else
	{
		reflect_prob = schlick(cos, eta);
	}
	static std::default_random_engine e;
	static std::uniform_real_distribution<float> u(0.0f, 1.0f);
	if (u(e) < reflect_prob)
	{
		scattered = Ray(rec.p, reflected, r_in.time());
	}
	else
	{
		scattered = Ray(rec.p, refracted, r_in.time());
	}

	return true;
}

float schlick(float cos, float refIndex)
{
	float r0 = (1 - refIndex) / (1 + refIndex);
	r0 = r0 * r0;
	return r0 + (1 - r0) * glm::pow(1 - cos, 5);
}