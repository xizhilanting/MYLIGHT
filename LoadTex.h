#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include<stb_image.h>
#include <ioStream>
#include<fstream>
#include "FFMpegRW\FFVideoRW.h"
#include "glImageData.h"
#include<set>
//通过filename输入纹理路径，绑定到textureID中进行返回（用unsigned int进行接收）。支持RGD\RGB\RGBA
static set<unsigned int> totalTex;
unsigned int LoadTex(std::string filename,unsigned int texNum=-1) {
	unsigned int textureID;
	if (!totalTex.count(texNum))
	{
		glGenTextures(1, &textureID);
		totalTex.insert(textureID);
	}
	else {
		return texNum;
	}

	stbi_set_flip_vertically_on_load(true);
	int width, height, nrComponents;
	unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
	//tbi_uc* stbi_load_from_memory(stbi_uc const* buffer, int len, int* x, int* y, int* comp, int req_comp)
	//STBIDEF stbi_uc* stbi_load(char const* filename, int* x, int* y, int* comp, int req_comp)
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

unsigned int LoadTex(CglImageData& imgdata, unsigned int texNum = -1) {

	unsigned int textureID;
	if (!totalTex.count(texNum))
	{
		glGenTextures(1, &textureID);
		totalTex.insert(textureID);
	}
	else {
		textureID = texNum;
	}
	
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
		}
		else if (nrComponents == 3)
		{
			format = GL_BGR;	//视频通常是BGR的
		}
		else if (nrComponents == 4)
		{
			format = GL_RGBA;
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
		//通过该方法读取的数据会在自己的结构体中回收，所以此处不需要free
		//stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << imgdata.GetSize() << std::endl;
		//stbi_image_free(data);
	}
	return textureID;
}