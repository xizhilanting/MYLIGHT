#pragma once
#include "glm/glm/glm.hpp"
#include "glm/glm/gtc/matrix_transform.hpp"
#include "glm/glm/gtc/type_ptr.hpp"
#include <vector>
#include "light.h"
//µ„π‚‘¥

class PointLight
{
public:
	PointLight();
	void addLight(pointLight& t_light) {
		light.push_back(t_light);
	}
	void addLight(glm::vec3& t_position) {
		pointLight t_light;
		t_light.position = t_position;
		addLight(t_light);
	}
	void clearLight()
	{
		light.clear();
	}
	std::vector<pointLight>
		getAllLight() {
		return light;
	}
	void changeLightPos(glm::vec3 pos, int i)
	{
		light[i].position = pos;
	}
	~PointLight();

private:
	std::vector<pointLight> light;
};

PointLight::PointLight()
{
}

PointLight::~PointLight()
{
}
