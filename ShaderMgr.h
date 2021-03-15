#pragma once
#include "Shader.h"
#include "glm/glm/glm.hpp"
#include "glm/glm/gtc/matrix_transform.hpp"
#include "glm/glm/gtc/type_ptr.hpp"
#include <map>
glm::mat4 shaderProject;
glm::mat4 shaderView;
glm::vec3 shaderViewPos;
struct ShaderStru
{
	Shader m_shader;
	glm::mat4 model;
	ShaderStru() {
		m_shader = Shader();
		model = glm::mat4();
	}
};
class ShaderMgr
{
public:
	ShaderMgr();
	~ShaderMgr();

	void setProject(glm::mat4 thproj)
	{
		shaderProject = thproj;
	}
	void setView( glm::mat4 thview)
	{
		shaderView = thview;
	}
	void setViewPos(glm::vec3 viewPos)
	{
		shaderViewPos = viewPos;
	}
	void setModel(Shader shader, glm::mat4 model)
	{
		content[shader.ID].m_shader = shader;
		content[shader.ID].model = model;
	}
	void transCom(Shader shader)
	{

	}
private:

	map<int , ShaderStru> content;
};

ShaderMgr::ShaderMgr()
{
}

ShaderMgr::~ShaderMgr()
{
}