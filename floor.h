#pragma once
#include "Shader.h"
#include "glm/glm/glm.hpp"
#include "glm/glm/gtc/matrix_transform.hpp"
#include "glm/glm/gtc/type_ptr.hpp"
#include "Camera.h"
#include <iostream>
#include <String>
using namespace std;

class Obgect {
private:
	string vPath, fPath;
	Shader *oShader;
	Obgect(const GLchar * vertexPath, const GLchar * fragmentPath):vPath(vertexPath),fPath(fragmentPath){ oShader=new Shader(vPath.c_str(), fPath.c_str()); }

};
