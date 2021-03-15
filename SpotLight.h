#pragma once
#include "glm/glm/glm.hpp"
#include "glm/glm/gtc/matrix_transform.hpp"
#include "glm/glm/gtc/type_ptr.hpp"
#include <vector>
#include "light.h"
class SpotLight
{
public:
	SpotLight();
	void addLight(spotLight& t_light) {
		light.push_back(t_light);
	}
	void addLight(glm::vec3& t_position) {
		spotLight t_light;
		t_light.position = t_position;
		addLight(t_light);

	}
	void clearLight()
	{
		light.clear();
	}
	std::vector<spotLight>
		getAllLight() {
		return light;
	}
	void setTar(glm::vec3 tar) {
		for (auto& i : light)
		{
			i.direction = tar - i.position;
			//cout << "�ı�" << tar.x << tar.y << tar.z << endl;
			//cout << "�ı�" << i.direction.x << i.direction.y << i.direction.z << endl;
		}
	}
	~SpotLight();

private:
	std::vector<spotLight> light;
};

SpotLight::SpotLight()
{
}

SpotLight::~SpotLight()
{
}
