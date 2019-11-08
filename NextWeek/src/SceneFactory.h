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
};

std::vector<std::function<Scene(float)>> getSceneFactories();

Scene scene1(float aspect);
Scene scene2(float aspect);