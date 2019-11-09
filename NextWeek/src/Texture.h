#pragma once
#include <memory>

#include "glm.hpp"

#include "Perlin.h"

class Texture
{
public:
	virtual glm::vec3 value(float u, float v, const glm::vec3& p) const = 0;
};

class ConstTexture : public Texture
{
public:
	ConstTexture(glm::vec3 color);

	glm::vec3 value(float u, float v, const glm::vec3& p) const override;

private:
	glm::vec3 m_color;
};

class CheckerTexture : public Texture
{
public:
	CheckerTexture(std::shared_ptr<Texture> t0, std::shared_ptr<Texture> t1, float scale = 5.0f);

	glm::vec3 value(float u, float v, const glm::vec3& p) const override;

private:
	std::shared_ptr<Texture> m_t0, m_t1;
	float m_scale;
};

class MarbleTexture : public Texture
{
public:
	MarbleTexture(float scale = 1.0f);

	glm::vec3 value(float u, float v, const glm::vec3& p) const override;

private:
	float m_scale;
	Perlin m_perlin;
};

class PerlinTexture : public Texture
{
public:
	PerlinTexture(float scale = 1.0f);

	glm::vec3 value(float u, float v, const glm::vec3& p) const override;

private:
	float m_scale;
	Perlin m_perlin;
};