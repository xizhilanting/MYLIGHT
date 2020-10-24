#pragma once
#include <glm/glm/glm.hpp>
#include "glm/glm/gtc/matrix_transform.hpp"
#include "glm/glm/gtc/type_ptr.hpp"

//vec×ª»¡¶È
float vec2radi(glm::vec3 dir) {
	float radi;
	if (dir.x == 0)
	{
		if (dir.z > 0)	radi = glm::radians(0.0f);
		else radi = glm::radians(180.0f);
		return radi;
	}
	else if (dir.z == 0)
	{
		if (dir.x > 0)	radi = glm::radians(90.0f);
		else radi = glm::radians(270.0f);
		return radi;
	}
	if (dir.x > 0)
		//radi = atan(dir.x / dir.z);
		radi = acos(dir.z / (sqrt(dir.x*dir.x + dir.z*dir.z)));
	else
		radi = acos(-dir.z / (sqrt(dir.x*dir.x + dir.z*dir.z))) + glm::radians(180.0f);
	return radi;

}