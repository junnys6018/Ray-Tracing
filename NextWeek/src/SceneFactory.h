#pragma once
#include <functional>
#include <vector>
#include <memory>

#include "Geometry.h"
#include "Camera.h"

struct Scene
{
	std::shared_ptr<Hitable> world;
	Camera cam;
	std::function<glm::vec3(glm::vec3)> skybox;
};

std::vector<std::function<Scene(float)>> getSceneFactories();

Scene scene1(float aspect);
Scene scene2(float aspect);
Scene scene3(float aspect);
Scene scene4(float aspect);
Scene scene5(float aspect);
Scene scene6(float aspect);