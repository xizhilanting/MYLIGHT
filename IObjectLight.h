#pragma once
#include "glm/glm/glm.hpp"
#include "glm/glm/gtc/matrix_transform.hpp"
#include "glm/glm/gtc/type_ptr.hpp"
#include "DirLight.h"
#include "PointLight.h"
#include <vector>
#include "Shader.h"
class IObjectLight
{
public:
	IObjectLight();
	void addDLight(dirLight& t_light)
	{
		dLight.addLight(t_light);
	}
	void addDLight(glm::vec3& t_position) {
		dirLight t_light;
		t_light.position = t_position;
		addDLight(t_light);
	}
	void clearDLight()
	{
		dLight.clearLight();
	}
	std::vector<dirLight>
		getAllDLight() {
		dLight.getAllLight();
	}

	void addPLight(pointLight& t_light)
	{
		pLight.addLight(t_light);
	}
	void addPLight(glm::vec3& t_position) {
		pointLight t_light;
		t_light.position = t_position;
		addPLight(t_light);
	}
	void clearPLight()
	{
		pLight.clearLight();
	}
	std::vector<dirLight>
		getAllPLight() {
		pLight.getAllLight();
	}
	~IObjectLight();
	void addShader(Shader t_shader)
	{
		shader.push_back(t_shader);
	}
	void changeLightPos(glm::vec3 pos,int i)
	{
		pLight.changeLightPos(pos, i);
	}
	void rendDLight() {
		for (auto t_shader:shader)
		{
			t_shader.use();
			for (auto light:dLight.getAllLight())
			{
				
				// directional light
				t_shader.setVec3("dirLight.direction", -light.position);
				t_shader.setVec3("dirLight.ambient", light.ambient);
				t_shader.setVec3("dirLight.diffuse", light.diffuse);
				t_shader.setVec3("dirLight.specular", light.specular);
			}
		}
	}
	void rendPLight() {
		for (auto t_shader : shader)
		{
			t_shader.use();
			for(unsigned int i = 0; i < pLight.getAllLight().size();i++)
			{
				auto light = pLight.getAllLight()[i];
				
				// directional light
				t_shader.setVec3(string("pointLights[") + to_string(i) + "].position", light.position);
				t_shader.setVec3("pointLights[" + to_string(i) + "].ambient", light.ambient);
				t_shader.setVec3("pointLights[" + to_string(i) + "].diffuse", light.diffuse);
				t_shader.setVec3("pointLights[" + to_string(i) + "].specular", light.specular);
				t_shader.setFloat("pointLights[" + to_string(i) + "].constant", light.constant);
				t_shader.setFloat("pointLights[" + to_string(i) + "].linear", light.linear);
				t_shader.setFloat("pointLights[" + to_string(i) + "].quadratic", light.quadratic);
			}
		}
	}
private:
	DirLight dLight;
	PointLight pLight;
	std::vector<Shader> shader;
};

IObjectLight::IObjectLight()
{
}

IObjectLight::~IObjectLight()
{
}