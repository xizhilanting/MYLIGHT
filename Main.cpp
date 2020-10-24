#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include <cstdlib>
#include <iostream>
#include "Shader.h"
#include "glm/glm/glm.hpp"
#include "glm/glm/gtc/matrix_transform.hpp"
#include "glm/glm/gtc/type_ptr.hpp"
#include "Camera.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Mode.h"
#include "floor.h"
#include<fstream>
#include "IObjectLight.h"
#include "LoadTex.h"
#include "GressD.h"
#include "vis.h"
#include "moveStep.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "vectexData.h"
using namespace std;
unsigned int amount = 1000;//��������
unsigned int peopleCount = 1;
float rate = 0;
float speed = 0.001;
int stepNum = 10;
bool FLLOWLEADER = false;
//���ڲ��ƶ�
moveStep movestep;

glm::vec3 curMousePos;
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
//void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void vis(bool firstControl);
void flloLeader();
glm::mat4* InitPlanetData();
GLFWwindow* RC();
unsigned int loadCubemap(vector<std::string> faces);
Camera MyCamera(glm::vec3(0, 50, -90));
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;
float screenWidth = 800, screenHeight = 600;//���ڳߴ�
//��ͼ�����ͶӰ����
glm::mat4 view;
glm::mat4 projection;

//�����ƹ������
IObjectLight IoLight;



// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;
int main()
{
	
	GLFWwindow* Window = RC();
	if (!Window)
	{
		return -1;
	}
	//������Ϣ
	Shader ourShader("Vertex.shader", "Fragment.shader");
	Shader lightCubeShader("Vertex.shader", "light.shader");
	Model ourModel("nanosuit/nanosuit.obj");
	Shader floorShader{ "Vertex.shader", "Fragment.shader" };
	//Shader BlendShader{ "Vertex.shader", "Blendfragment.shader" };
	Shader skyboxShader{ "skycubeVertex.shader" ,"skycubeFragment.shader" };

	GressD gressD(&MyCamera);
	gressD.addPos(glm::vec3(-1.0, 0.5, -3));
	gressD.addPos(glm::vec3(-1.0, 0.5, -6));
	gressD.addPos(glm::vec3(-1.0, 0.5, -9));
	gressD.addPos(glm::vec3(-1.0, 0.5, -16));
	IoLight.addShader(gressD.getShader());
	IoLight.addShader(ourShader);
	//IoLight.addShader(BlendShader);
	IoLight.addShader(floorShader);

	//��պ�
	vector<std::string> faces
	{
		"skybox//right.jpg",
		"skybox//left.jpg",
		"skybox//top.jpg",
		"skybox//bottom.jpg",
		"skybox//front.jpg",
		"skybox//back.jpg"
	};
	unsigned int cubemapTexture = loadCubemap(faces);
	unsigned int skyboxVBO;
	glGenBuffers(1, &skyboxVBO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW);
	unsigned int skyboxVAO;
	glGenVertexArrays(1, &skyboxVAO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//��պм��ؽ���

	
	//���õ�
	vector<pointLight> pointLight(4);
	for (unsigned int  i = 0; i < 4; i++)
	{
		pointLight[i].position = pointLightPositions[i];
		IoLight.addPLight(pointLight[i]);
	}
	dirLight LdirLight;
	LdirLight.position = glm::vec3(25.0f, 30.0f, -70.0f);
	IoLight.addDLight(LdirLight);
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	unsigned int lightCubeVAO;
	glGenVertexArrays(1, &lightCubeVAO);
	glBindVertexArray(lightCubeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	//�ذ�
	unsigned int textureIDFloor = LoadTex("wall.jpg");
	unsigned int floorVBO;
	glGenBuffers(1, &floorVBO);
	glBindBuffer(GL_ARRAY_BUFFER, floorVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vecFloord), vecFloord, GL_STATIC_DRAW);
	unsigned int floorVAO;
	glGenVertexArrays(1, &floorVAO);
	glBindVertexArray(floorVAO);
	glBindBuffer(GL_ARRAY_BUFFER, floorVBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
	
	
	// �ڴ�֮ǰ��Ҫ�������� use ��Ӧ����ɫ���������趨uniform��
	ourShader.use();
	ourShader.setFloat("material.shininess", 64.0f);
	floorShader.use();
	floorShader.setFloat("material.shininess", 64.0f);
	//BlendShader.use();
	//BlendShader.setFloat("material.shininess", 64.0f);
	//�ӿڿ�
		//��shader�е�uniform
	unsigned int uniformOur = glGetUniformBlockIndex(ourShader.ID, "Matrices");
	unsigned int uniformLight = glGetUniformBlockIndex(lightCubeShader.ID, "Matrices");
	unsigned int uniformFloor = glGetUniformBlockIndex(floorShader.ID, "Matrices");
	//unsigned int uniformBlend = glGetUniformBlockIndex(BlendShader.ID, "Matrices");
	glUniformBlockBinding(ourShader.ID, uniformOur,4);
	glUniformBlockBinding(lightCubeShader.ID, uniformLight, 4);
	glUniformBlockBinding(floorShader.ID, uniformFloor, 4);
	//glUniformBlockBinding(BlendShader.ID, uniformBlend, 4);
		//����һ���յĽӿڿ�
	unsigned int uboMatrices;
	glGenBuffers(1, &uboMatrices);
	glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
	glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	glBindBufferRange(GL_UNIFORM_BUFFER, 4, uboMatrices, 0, 2 * sizeof(glm::mat4));
	
	
	while (!glfwWindowShouldClose(Window))
	{
		//��������
		processInput(Window);
		
		
		glEnable(GL_DEPTH_TEST);
		//��Ⱦ
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		//test...
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		
		//���ù��õ���������
		view = MyCamera.GetViewMatrix();
		projection= glm::perspective(glm::radians(MyCamera.Zoom), (float)screenWidth / (float)screenHeight, 0.1f, 1000.0f);
		//����uniform��
		glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(projection));
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
		glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
		glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(view));
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	
		//���ù��λ��
		static float rotOld = 0;
		float rot = glfwGetTime() - rotOld;
		if (rot>=10)
		{
			rotOld = glfwGetTime();
		}
		rot *= 10;

		for (unsigned int i = 0; i < 4; i++)
		{
			lightCubeShader.use();
			lightCubeShader.setVec3("lightColor", pointLight[0].specular+ pointLight[0].ambient);
			glm::mat4 model;
			/*model = glm::rotate(model, 100*glm::radians(rot), glm::vec3(0, 1, 0));*/
			model = glm::translate(model, glm::vec3(0.0f, 0.0f, -rot));
			model = glm::translate(model, pointLightPositions[i]);
			model = glm::translate(model,glm::vec3(0,8,0));
			lightCubeShader.setMat4("model", model);
			glDrawArrays(GL_TRIANGLES, 0, 36);	
			//�ѹ�Դͬ����ģ����	
			IoLight.rendDLight();
			IoLight.changeLightPos(model*glm::vec4(0, 0, 0, 1), i);
			IoLight.rendPLight();
		}
		//���ù����
		//draw ģ��
		ourShader.use();
		ourShader.setVec3("viewPos", MyCamera.Position);
		//���ð�Χ��
		babox  box = ourModel.getbox();
		movestep.setBox(box.x.max, box.y.max, box.z.max,
			box.x.min, box.y.min, box.z.min);

		static bool first = true;
		for (unsigned int i=0;i<peopleCount;i++)
		{
				if (first)
				{
					glm::mat4 model;
					
					//model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
					model = glm::translate(model, glm::vec3(9.0f*(i % 10), 0.0f, -7.0f*(i / 10)));
					movestep.addInstance(model);

					ourShader.setMat4("model", model);
					ourModel.Draw(ourShader);
				}
				else
				{
					glm::mat4 model;
					//model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
					/*glm::vec3 tmp = movestep.getCurModel(i)*glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
					model = glm::translate(model, tmp);*/
					ourShader.setMat4("model", movestep.getCurModel(i));
					ourModel.Draw(ourShader);
				}
				
		}
		if (first)
		{
			first = !first;
		}
		//�ذ�
		floorShader.use();
		glActiveTexture(GL_TEXTURE0);
		floorShader.setFloat("material.diffuse", 0);
		glBindTexture(GL_TEXTURE_2D, textureIDFloor);
		glActiveTexture(GL_TEXTURE1);
		floorShader.setFloat("material.specular", 1);
		glBindTexture(GL_TEXTURE_2D, textureIDFloor);
		floorShader.setVec3("viewPos", MyCamera.Position);
		int row = 50, col = 50;
		float rate = 5;
		glBindVertexArray(floorVAO);
		for (unsigned int i = 0; i < row; i++)
		{
			for (unsigned int j = 0; j < col; j++)
			{
				glm::mat4 model;
				model = glm::scale(model, glm::vec3(rate, 1, rate));
				model = glm::translate(model, glm::vec3(1.0f*i, -0.5f, -1.0f*j));
				floorShader.setMat4("model", model);
				glDrawArrays(GL_TRIANGLES, 0, 6);
			}
		}
		

		//���ݿ��Ա���
		glm::mat4 gressScale;
		gressScale = glm::scale(gressScale, glm::vec3(5, 25, 5));
		gressD.setModel(gressScale);
		gressD.doDraw();
		

		
		//��պл���
		glDepthMask(GL_FALSE);
		glDepthFunc(GL_LEQUAL);
		skyboxShader.use();
		// ... ���ù۲��ͶӰ����
		skyboxShader.setMat4("projection", projection);
		glm::mat4 skyView = glm::mat4(glm::mat3(view));
		skyboxShader.setMat4("view", skyView);
		glBindVertexArray(skyboxVAO);
		glActiveTexture(GL_TEXTURE0);
		//BlendShader.setFloat("skybox", 0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glDepthMask(GL_TRUE);
		glDepthFunc(GL_LESS);
			//����

		
		
		glfwSwapBuffers(Window);
		glfwPollEvents();
	}
	
	glDeleteBuffers(1, &VBO);
	glfwTerminate();
	return 0;
}

//���ڻص�����
void framebuffer_size_callback(GLFWwindow* Window, int width, int height)
{
	glViewport(0, 0, width, height);
}
//��������
void processInput(GLFWwindow* Window)
{
	float currentFrame = glfwGetTime();
	static bool firstControl = true;
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;
	float cameraSpeed = 3.5f * deltaTime;
	if (glfwGetKey(Window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(Window, true);
	if (glfwGetKey(Window, GLFW_KEY_F1) == GLFW_PRESS)
	{
		firstControl = !firstControl;
		vis(firstControl);
	}
	if (firstControl)
	{
		if (glfwGetKey(Window, GLFW_KEY_W) == GLFW_PRESS)
			MyCamera.ProcessKeyboard(FORWARD, deltaTime);
		if (glfwGetKey(Window, GLFW_KEY_S) == GLFW_PRESS)
			MyCamera.ProcessKeyboard(BACKWARD, deltaTime);
		if (glfwGetKey(Window, GLFW_KEY_A) == GLFW_PRESS)
			MyCamera.ProcessKeyboard(LEFT, deltaTime);
		if (glfwGetKey(Window, GLFW_KEY_D) == GLFW_PRESS)
			MyCamera.ProcessKeyboard(RIGHT, deltaTime);
		if (glfwGetKey(Window, GLFW_KEY_Q) == GLFW_PRESS)
			MyCamera.ProcessKeyboard(DOWN, deltaTime);
		if (glfwGetKey(Window, GLFW_KEY_E) == GLFW_PRESS)
			MyCamera.ProcessKeyboard(UP, deltaTime);
	} 
	else
	{
		if (glfwGetKey(Window, GLFW_KEY_W) == GLFW_PRESS)
			MyCamera.ProcessKeyboard(UP, deltaTime);
		if (glfwGetKey(Window, GLFW_KEY_S) == GLFW_PRESS)
			MyCamera.ProcessKeyboard(DOWN, deltaTime);
		if (glfwGetKey(Window, GLFW_KEY_A) == GLFW_PRESS)
			MyCamera.ProcessKeyboard(LEFT, deltaTime);
		if (glfwGetKey(Window, GLFW_KEY_D) == GLFW_PRESS)
			MyCamera.ProcessKeyboard(RIGHT, deltaTime);
		if (glfwGetKey(Window, GLFW_KEY_Q) == GLFW_PRESS)
			MyCamera.ProcessKeyboard(FORWARD, deltaTime);
		if (glfwGetKey(Window, GLFW_KEY_E) == GLFW_PRESS)
			MyCamera.ProcessKeyboard(BACKWARD, deltaTime);
	}
	if (glfwGetKey(Window, GLFW_KEY_2) == GLFW_PRESS)
		MyCamera.ProcessKeyboard(SLOWER, deltaTime);
	if (glfwGetKey(Window, GLFW_KEY_3) == GLFW_PRESS)
		MyCamera.ProcessKeyboard(FASTER, deltaTime);
	if (glfwGetKey(Window, GLFW_KEY_4) == GLFW_PRESS)
		MyCamera.ProcessKeyboard(TURNSLOWER, deltaTime);
	if (glfwGetKey(Window, GLFW_KEY_5) == GLFW_PRESS)
		MyCamera.ProcessKeyboard(TURNFASTER, deltaTime);
	if (glfwGetKey(Window, GLFW_KEY_I) == GLFW_PRESS)
		MyCamera.ProcessKeyboard(LOOKUP, deltaTime);
	if (glfwGetKey(Window, GLFW_KEY_K) == GLFW_PRESS)
		MyCamera.ProcessKeyboard(LOOKDOWN, deltaTime);
	if (glfwGetKey(Window, GLFW_KEY_J) == GLFW_PRESS)
		MyCamera.ProcessKeyboard(LOOKLEFT, deltaTime);
	if (glfwGetKey(Window, GLFW_KEY_L) == GLFW_PRESS)
		MyCamera.ProcessKeyboard(LOOKRIGHT, deltaTime);
	if (glfwGetKey(Window, GLFW_KEY_SPACE) == GLFW_PRESS)
		movestep.changeLookMode();
	if (glfwGetKey(Window, GLFW_KEY_UP) == GLFW_PRESS)
		movestep.moveCir(0, 1);
	if (glfwGetKey(Window, GLFW_KEY_DOWN) == GLFW_PRESS)
		movestep.moveCir(0, -1);

	if (glfwGetKey(Window, GLFW_KEY_LEFT) == GLFW_PRESS)
		movestep.moveCir(1, 0);
	if (glfwGetKey(Window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		movestep.moveCir(-1, 0);
	if (glfwGetKey(Window, GLFW_KEY_Z) == GLFW_PRESS)
		FLLOWLEADER = !FLLOWLEADER;

}
//ͨ�����ı��ӽ�
//void mouse_callback(GLFWwindow* window, double xpos, double ypos)
//{
//	if (firstMouse)
//	{
//		lastX = xpos;
//		lastY = ypos;
//		firstMouse = false;
//	}
//
//	float xoffset = xpos - lastX;
//	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
//
//	lastX = xpos;
//	lastY = ypos;
//
//	MyCamera.ProcessMouseMovement(xoffset, yoffset);
//}
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	MyCamera.ProcessMouseScroll(yoffset);
}
//����ͨ��faces�����ͼƬ��ɵ���պ�ID
unsigned int loadCubemap(vector<std::string> faces)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
	stbi_set_flip_vertically_on_load(false);
	int width, height, nrChannels;
	for (unsigned int i=0;i<faces.size()&&i<6;i++)
	{
		unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
			stbi_image_free(data);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return textureID;
}

GLFWwindow* RC() {
	//��ʼ��
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow* Window = glfwCreateWindow(800, 600, "LearnOpengl", NULL, NULL);
	if (!Window)
	{
		cout << "faild" << endl;
		glfwTerminate();
		return nullptr;
	}
	glfwMakeContextCurrent(Window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return nullptr;
	}
	//�󶨻ص�����
	glfwSetFramebufferSizeCallback(Window, framebuffer_size_callback);
	//glfwSetInputMode(Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	//glfwSetCursorPosCallback(Window, mouse_callback);
	glfwSetScrollCallback(Window, scroll_callback);
	//glfwSetCursorPosCallback(Window, curse_poscallback);//��ȡ��굱ǰλ�ûص�
	glfwSetMouseButtonCallback(Window, mouse_button_callback);//������ص�
	glEnable(GL_DEPTH_TEST | GL_STENCIL_TEST);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	vis(true);
	return Window;
}
float calcCo(glm::vec3 begin, glm::vec3 end) {
	return glm::acos(glm::dot(begin, end) / movestep.calmoc(begin)*movestep.calmoc(begin));
}
//��i����ΪԲ������Ϊԭ��

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	//ͨ����������ת��Ϊ��������
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		double xpos, ypos;
		//getting cursor position 
		glfwGetCursorPos(window, &xpos, &ypos);
		float val;
		ypos = SCR_HEIGHT - ypos;
		
		glReadPixels(xpos, ypos, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &val);

		curMousePos.x = xpos;
		curMousePos.y = ypos;
		curMousePos.z = val;
		glm::vec3 worldPos;
		worldPos = glm::unProject(curMousePos, view, projection, glm::vec4(0,0,800,600));
		/*cout << "Cursor Position at (" << curMousePos.x << " : " << curMousePos.y << " : " <<curMousePos.z<<")"<<endl;
		cout << "World Position at (" << worldPos.x << " : " << worldPos.y << " : " << worldPos.z << ")" << endl<<endl;*/
		babox box = movestep.getBox();
		for (int i=0;i<movestep.getNum();i++)
		{
			
			glm::vec4 tmp=glm::inverse(movestep.getCurModel(i))*glm::vec4(worldPos, 1);
			bool x = tmp.x >= box.x.min&&tmp.x <= box.x.max;
			bool y = tmp.y >= box.y.min&&tmp.y <= box.y.max;
			bool z = tmp.z >= box.z.min&&tmp.z <= box.z.max;
			if (x&&y&&z)
			{
				cout << "���ڵ�" << i << endl;
				movestep.setCPoint( i);
				break;
			}
		}
	}
}

glm::mat4 * InitPlanetData()
{
	
	glm::mat4 *modelMatrices;
	modelMatrices = new glm::mat4[amount];
	srand(glfwGetTime()); // ��ʼ���������    
	float radius = 40.0;
	float offset = 7.0f;
	for (unsigned int i = 0; i < amount; i++)
	{
		glm::mat4 model;
		// 1. λ�ƣ��ֲ��ڰ뾶Ϊ 'radius' ��Բ���ϣ�ƫ�Ƶķ�Χ�� [-offset, offset]
		float angle = (float)i / (float)amount * 360.0f;
		float displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
		float x = sin(angle) * radius + displacement;
		displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
		float y = displacement * 0.4f; // �����Ǵ��ĸ߶ȱ�x��z�Ŀ��ҪС
		displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
		float z = cos(angle) * radius + displacement;
		model = glm::translate(model, glm::vec3(x, y, z));

		// 2. ���ţ��� 0.05 �� 0.25f ֮������
		float scale = (rand() % 20) / 100.0f + 0.05;
		model = glm::scale(model, glm::vec3(scale));

		// 3. ��ת������һ�����룩���ѡ�����ת�����������������ת
		float rotAngle = (rand() % 360);
		model = glm::rotate(model, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));

		// 4. ��ӵ������������
		modelMatrices[i] = model;
	}
	return modelMatrices;
}

