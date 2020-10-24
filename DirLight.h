#pragma once
#include "glm/glm/glm.hpp"
#include "glm/glm/gtc/matrix_transform.hpp"
#include "glm/glm/gtc/type_ptr.hpp"
#include <vector>
#include "light.h"
class DirLight
{
public:
	DirLight();
	void addLight(dirLight& t_light) {
		light.push_back(t_light);
	}
	void addLight(glm::vec3& t_position) {
		dirLight t_light;
		t_light.position= t_position;
		addLight(t_light);

	}
	void clearLight()
	{
		light.clear();
	}
	std::vector<dirLight>
		getAllLight() {
		return light;
	}
	~DirLight();

private:
	std::vector<dirLight> light;
};

DirLight::DirLight()
{
}

DirLight::~DirLight()
{
}
