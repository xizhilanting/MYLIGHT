#pragma once
#include <glm/glm/glm.hpp>
struct dirLight {
	glm::vec3 position;
	glm::vec3 ambient{ 0.5f, 0.5f, 0.5f };
	glm::vec3 diffuse{ 0.8f, 0.8f, 0.8f };
	glm::vec3 specular{ 1.0f,1.0f,1.0f };
};
struct pointLight {
	glm::vec3 position;
	glm::vec3 ambient{ 0.05f, 0.05f, 0.05f };
	glm::vec3 diffuse{ 0.8f, 0.8f, 0.8f };
	glm::vec3 specular{ 0.0f,1.0f,1.0f };
	float constant = 1.0f;
	float linear = 0.09;
	float quadratic = 0.032;
};
struct spotLight
{
	glm::vec3 position{ 0,15,0 };
	glm::vec3 direction{ 0,-1.0f,0 };
	glm::vec3 ambient{ 0.0f, 0.0f, 0.0f };
	glm::vec3 diffuse{ 0.0f, 1.0f, 1.0f };
	glm::vec3 specular{ 0.0f, 1.0f, 1.0f };
	float constant = 1.0f;
	float linear = 0.023;
	float quadratic = 0.032;
	float cutOff = glm::cos(glm::radians(11.0f));
	float outerCutOff = glm::cos(glm::radians(12.0f));
	
};
//点光源位置
glm::vec3 pointLightPositions[] = {
	glm::vec3(6.0f,		10.0f,	0.0f),
	glm::vec3(12.0f,	10.0f,	0.0f),
	glm::vec3(18.0f,	10.0f,	0.0f),
	glm::vec3(24.0f,	10.0f,	0.0f)
};
