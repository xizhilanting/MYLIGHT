#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include<stb_image.h>
#include <ioStream>
#include<fstream>
#include "FFMpegRW\FFVideoRW.h"
#include "glImageData.h"
//ͨ��filename��������·�����󶨵�textureID�н��з��أ���unsigned int���н��գ���֧��RGD\RGB\RGBA
unsigned int LoadTex(std::string filename) {
	unsigned int textureID;
	glGenTextures(1, &textureID);
	stbi_set_flip_vertically_on_load(true);
	int width, height, nrComponents;
	unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
	//tbi_uc* stbi_load_from_memory(stbi_uc const* buffer, int len, int* x, int* y, int* comp, int req_comp)
	//STBIDEF stbi_uc* stbi_load(char const* filename, int* x, int* y, int* comp, int req_comp)
	if (data)
	{
		static bool first = true;
		if (first)
		{
			first = false;
			ofstream fp("a.dat", std::ios::binary);
			fp.write((const char*)data, sizeof(unsigned char)*10000);
			fp.close();
			cout << filename << " write down!" << endl;
		}
		GLenum format;
		if (nrComponents == 1)
		{
			format = GL_RED;
			//cout << "RED" << endl;
		}
		else if (nrComponents == 3)
		{
			format = GL_RGB;
			//cout << "rgb" << endl;
		}
		else if (nrComponents == 4)
		{
			format = GL_RGBA;
			//cout << "rgba" << endl;
		}

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		/*glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);*/
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << filename << std::endl;
		stbi_image_free(data);
	}
	return textureID;
}

unsigned int LoadTex(CglImageData &imgdata) {
	//static bool first = true;
	//static unsigned int textureID;
	//if (first) {
	//	glGenTextures(1, &textureID);
	//	first = false;
	//}
	unsigned int textureID;
	glGenTextures(1, &textureID);
	//stbi_set_flip_vertically_on_load(true);
	int width = imgdata.m_Width;
	int height=imgdata.m_Height;
	int nrComponents=imgdata.m_nChan;
	unsigned char* data = imgdata.m_pData;
	if (data)
	{

		GLenum format;
		if (nrComponents == 1)
		{
			format = GL_RED;
			//cout << "RED" << endl;
		}
		else if (nrComponents == 3)
		{
			format = GL_BGR;
			//cout << "rgb" << endl;
		}
		else if (nrComponents == 4)
		{
			format = GL_RGBA;
			//cout << "rgba" << endl;
		}

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);


		/*glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);*/

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		//ͨ���÷�����ȡ�����ݻ����Լ��Ľṹ���л��գ����Դ˴�����Ҫfree
		//stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << imgdata.GetSize() << std::endl;
		//stbi_image_free(data);
	}
	return textureID;
}