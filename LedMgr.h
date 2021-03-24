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
//*******************完成
//使用方法：首先add主绘制shader，然后可以在主绘制shader下加载led绘制shader，每个led绘制shader加载一个或多个LedStru结构，用于描述LED
//然后再主循环渲染部分draw，但是D和S设置效果不明显暂时使用整体draw
extern glm::vec3 ins;
extern int density;
extern float cutOff;
extern float outerCutOff;
struct LedStru
{
	glm::mat4 model;
	unsigned int tex;
	unsigned int VAO;
	LedStru(glm::mat4 thmodel,unsigned int thtex,unsigned int thVAO):model(thmodel),tex(thtex),VAO(thVAO){}
};
class LedMgr
{
public:
	LedMgr();
	~LedMgr();
	void addMShader(Shader &shader)
	{
		if (!m_Mshader.count(shader.ID))
			m_Mshader[shader.ID];
	}
	void addLShader(Shader &Mshader,Shader &Lshader)
	{
		if (m_Mshader.count(Mshader.ID))
		{
			if (!m_Lshader.count(Lshader.ID))
			{
				m_Lshader[Lshader.ID];
			}
			auto itor = m_Lshader.find(Lshader.ID);
			m_Mshader[Mshader.ID][Lshader.ID] = itor;
		}
		else
		{
			if (!m_Lshader.count(Lshader.ID))
			{
				m_Lshader[Lshader.ID];
			}
			auto itor = m_Lshader.find(Lshader.ID);
			m_Mshader[Mshader.ID][Lshader.ID] = itor;
		}
	}
	void addStru(Shader &Lshader, LedStru &stru)
	{
		if (!m_Lshader.count(Lshader.ID))
		{
			m_Lshader[Lshader.ID].push_back(stru);
		}
		else {
			m_Lshader[Lshader.ID].push_back(stru);
		}
	}
	void printSize() {
		cout << "LEDmGRSIZE= "<<m_Mshader.size() << " " << m_Lshader.size() << " " << m_Lshader.begin()->second.size() << endl;
	}
	bool DrawLedS(Shader &Mshader)
	{
		for (auto MshaderSecItor : m_Mshader[Mshader.ID])
		{
			Mshader.use();
			for (auto LshaderItor : MshaderSecItor.second->second)
			{
				Mshader.use();
				Mshader.setVec3("LEDA.ld", LshaderItor.model* glm::vec4(-0.5f, 0.5f, 0.5f, 1.0f));
				Mshader.setVec3("LEDA.rd", LshaderItor.model * glm::vec4(0.5f, 0.5f, 0.5f, 1.0f));
				Mshader.setVec3("LEDA.ru", LshaderItor.model * glm::vec4(0.5f, 0.5f, -0.5f, 1.0f));
				Mshader.setVec3("LEDA.ins", ins);	
				Mshader.setInt("LEDA.density", density);
				Mshader.setFloat("LEDA.cutOff", cutOff);
				Mshader.setFloat("LEDA.outerCutOff", outerCutOff);
			}
		}
		return true;
	}
	bool DrawLedD(Shader &Mshader)
	{
		int texIndex = 0;
		for (auto MshaderSecItor : m_Mshader[Mshader.ID])
		{
			Mshader.use();
			Mshader.setFloat("LEDA.SLED", texIndex++);
			Shader Lshadertmp;
			Lshadertmp.ID = MshaderSecItor.first;
			for (auto LshaderItor : MshaderSecItor.second->second)
			{
				Mshader.use();
				glBindTexture(GL_TEXTURE_2D, LshaderItor.tex);
				
				Mshader.setVec3("LEDA.ins", ins);
				Mshader.setInt("LEDA.density", density);
				Mshader.setFloat("LEDA.cutOff", cutOff);
				Mshader.setFloat("LEDA.outerCutOff", outerCutOff);
				Lshadertmp.use();
				//*************************************工作到此***************************
				//***********************************需要当shader管理类完成*************
				glActiveTexture(GL_TEXTURE0);
				Lshadertmp.setFloat("material.diffuse", 0);
				glBindTexture(GL_TEXTURE_2D, LshaderItor.tex);
				glActiveTexture(GL_TEXTURE1);
				Lshadertmp.setFloat("material.specular", 1);
				glBindTexture(GL_TEXTURE_2D, LshaderItor.tex);
				glBindVertexArray(LshaderItor.VAO);
				Lshadertmp.setMat4("model", LshaderItor.model);
				glDrawArrays(GL_TRIANGLES, 0, 6);
			}
		}
		return true;
	}
	bool DrawLed(Shader &Mshader)
	{
		int texIndex = 0;
		int be = 0;
		for (auto MshaderSecItor : m_Mshader[Mshader.ID])
		{
			Shader Lshadertmp;
			Lshadertmp.ID = MshaderSecItor.first;
			for (auto LshaderItor : MshaderSecItor.second->second)
			{
				Mshader.use();
				Mshader.setInt("LEDA[" + to_string(texIndex)+"].SLED", texIndex+ be);
				glActiveTexture(GL_TEXTURE0+texIndex + be);
				glBindTexture(GL_TEXTURE_2D, LshaderItor.tex);
				Mshader.setVec3("LEDA[" + to_string(texIndex) + "].ld", LshaderItor.model * glm::vec4(-0.5f, 0.5f, 0.5f, 1.0f));
				Mshader.setVec3("LEDA[" + to_string(texIndex) + "].rd", LshaderItor.model * glm::vec4(0.5f, 0.5f, 0.5f, 1.0f));
				Mshader.setVec3("LEDA[" + to_string(texIndex) + "].ru", LshaderItor.model * glm::vec4(0.5f, 0.5f, -0.5f, 1.0f));
				Mshader.setVec3("LEDA[" + to_string(texIndex) + "].ins", ins);
				Mshader.setInt("LEDA[" + to_string(texIndex) + "].density", density);
				Mshader.setFloat("LEDA[" + to_string(texIndex) + "].cutOff", cutOff);
				Mshader.setFloat("LEDA[" + to_string(texIndex) + "].outerCutOff", outerCutOff);
				texIndex++;
				//be++;
				//*************************************工作到此***************************
				//***********************************需要当shader管理类完成*************
				Lshadertmp.use();
				glActiveTexture(GL_TEXTURE0);
				Lshadertmp.setInt("material.diffuse", 0);
				glBindTexture(GL_TEXTURE_2D, LshaderItor.tex);
				//glActiveTexture(GL_TEXTURE1);
				//Lshadertmp.setInt("material.specular", 1);
				//glBindTexture(GL_TEXTURE_2D, LshaderItor.tex);
				//Lshadertmp.setVec3("viewPos", MyCamera.Position);
				glBindVertexArray(LshaderItor.VAO);
				Lshadertmp.setMat4("model", LshaderItor.model);
				glDrawArrays(GL_TRIANGLES, 0, 6);
			}
		}
		//cout << endl;
		return true;
	}
	void clear(unsigned int Lshaderid)
	{
		m_Lshader[Lshaderid].clear();
	}
private:
	map<unsigned int, map<unsigned int, map<unsigned int, vector<LedStru>>::iterator>> m_Mshader;
	map<unsigned int, vector<LedStru>> m_Lshader;
};

LedMgr::LedMgr()
{
}

LedMgr::~LedMgr()
{
}