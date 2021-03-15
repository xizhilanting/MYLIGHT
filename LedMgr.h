#pragma once
#include <vector>
#include "Shader.h"
#include <map>
#include <set>
#include "glm/glm/glm.hpp"
#include "glm/glm/gtc/matrix_transform.hpp"
#include "glm/glm/gtc/type_ptr.hpp"
//面光源管理类，添加面光源shader，总渲染shader，面光源model，然后进行渲染。
//*********未完成
extern glm::vec3 ins;
struct LedStru
{
	glm::mat4 model;
	int tex;
};
class LedMgr
{
public:
	LedMgr();
	~LedMgr();
	void addMShader(Shader shader)
	{
		if (!m_Mshader.count(shader))
			m_Mshader[shader];
	}
	void addLShader(Shader Mshader,Shader Lshader)
	{
		if (m_Mshader.count(Mshader))
		{
			if (!m_Lshader.count(Lshader))
			{
				m_Lshader[Lshader];
			}
			auto itor = m_Lshader.find(Lshader);
			m_Mshader[Mshader][Lshader] = itor;
		}
		else
		{
			if (!m_Lshader.count(Lshader))
			{
				m_Lshader[Lshader];
			}
			auto itor = m_Lshader.find(Lshader);
			m_Mshader[Mshader][Lshader] = itor;
		}
	}
	void addStru(Shader Lshader, LedStru stru)
	{
		if (!m_Lshader.count(Lshader))
		{
			m_Lshader[Lshader].insert(stru);
		}
		else {
			m_Lshader[Lshader].insert(stru);
		}
	}
	bool DrawLed(Shader Mshader)
	{
		int texIndex = 0;
		Mshader.use();
		for (auto MshaderSecItor : m_Mshader[Mshader])
		{
			Mshader.setFloat("LEDA.SLED", texIndex++);
			Shader Lshadertmp = MshaderSecItor.first;
			for (auto LshaderItor : MshaderSecItor.second->second)
			{
				glBindTexture(GL_TEXTURE_2D, LshaderItor.tex);
				Mshader.setVec3("LEDA.ld", LshaderItor.model* glm::vec4(-0.5f, 0.5f, 0.5f, 1.0f));
				Mshader.setVec3("LEDA.rd", LshaderItor.model * glm::vec4(0.5f, 0.5f, 0.5f, 1.0f));
				Mshader.setVec3("LEDA.ru", LshaderItor.model * glm::vec4(0.5f, 0.5f, -0.5f, 1.0f));
				Mshader.setVec3("LEDA.ins", ins);
				Lshadertmp.use();
				//*************************************工作到此***************************
				//***********************************需要当shader管理类完成*************
				glActiveTexture(GL_TEXTURE0);
				Lshadertmp.setFloat("material.diffuse", 0);
				glBindTexture(GL_TEXTURE_2D, LshaderItor.tex);
				glActiveTexture(GL_TEXTURE1);
				Lshadertmp.setFloat("material.specular", 1);
				glBindTexture(GL_TEXTURE_2D, LshaderItor.tex);
				//Lshadertmp.setVec3("viewPos", MyCamera.Position);
				//glBindVertexArray(floorVAO);
				Lshadertmp.setMat4("model", LshaderItor.model);
			}
		}
		
	}
private:
	map<Shader, map<Shader, map<Shader, set<LedStru>>::iterator>> m_Mshader;
	map<Shader, set<LedStru>> m_Lshader;
};

LedMgr::LedMgr()
{
}

LedMgr::~LedMgr()
{
}