#pragma once
#include "glm/glm/glm.hpp"
#include "glm/glm/gtc/matrix_transform.hpp"
#include "glm/glm/gtc/type_ptr.hpp"
#include "LoadTex.h"
#include <vector>
#include "Camera.h"
#include "Shader.h"
#include <iostream>
#include"vectexData.h"
using namespace std;

class GressD
{
private:
public:
	GressD(Camera* camera) { 
		myCamera = camera;
	projection = glm::perspective(glm::radians(myCamera->Zoom), (float)800 / (float)600, 0.1f, 100.0f);
	G_shader = new Shader{ "Vertex.shader", "RGBAfragment.shader" };
	
	}
	void setup() {
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vec_Gress), vec_Gress, GL_STATIC_DRAW);
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		glEnableVertexAttribArray(2);
		glBindVertexArray(0);
		TexID= LoadTex("grass.png");
		G_shader->use();
		glActiveTexture(GL_TEXTURE0);
		G_shader->setFloat("material.diffuse", 0);
		glBindTexture(GL_TEXTURE_2D, TexID);
		glActiveTexture(GL_TEXTURE1);
		G_shader->setFloat("material.specular", 1);
		glBindTexture(GL_TEXTURE_2D, TexID);
		G_shader->setFloat("material.shininess", 64.0f);

	}
	~GressD();
	void addPos(glm::vec3 pos)
	{
		position.push_back(pos);
	}
	void doDraw() {
		setup();
		G_shader->use();
		glBindVertexArray(VAO);
		G_shader->setMat4("projection", projection);
		G_shader->setMat4("view",myCamera->GetViewMatrix());
		G_shader->setVec3("viewPos", myCamera->Position);
		//glDisable(GL_CULL_FACE);
		for (auto pos:position)
		{
			glm::mat4 pmodel;
			pmodel = model;
			pmodel = glm::translate(pmodel, pos);
			pmodel = glm::rotate(pmodel, glm::radians(90.0f), glm::vec3(1, 0, 0));
			for (unsigned int i=0;i<4;i++)
			{
				pmodel = glm::rotate(pmodel, glm::radians(45.0f), glm::vec3(0, 0, 1));
				G_shader->setMat4("model", pmodel);
				
				glDrawArrays(GL_TRIANGLES, 0, 6);
				
			}
		}
		//model = glm::mat4(1);
		//glEnable(GL_CULL_FACE);
	}
	void setModel(glm::mat4 t_model)
	{
		model = t_model;
	}
	void setTexID(unsigned int ID) {
		TexID = ID;
	}
	Shader& getShader() {
		return *G_shader;
	}
private:
	unsigned int TexID;
	unsigned int VBO;
	unsigned int VAO;
	Shader *G_shader;
	Camera *myCamera;
	glm::mat4 projection, model;
	glm::vec3 viewPos;
	std::vector<glm::vec3> position;
};


GressD::~GressD()
{
}
