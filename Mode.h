#pragma once
#include "Mesh.h"
#include <vector>
#include <string>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Shader.h"
#include<stb_image.h>
#include "LoadTex.h"
#include "moveStep.h"
unsigned int TextureFromFile(const char* path, string	directory);

class Model
{
public:

	Model(const char* path)
	{
		loadMode(path);
	}
	void Draw(Shader &shader)
	{
		for (unsigned int i = 0; i < meshes.size(); i++)
		{
			meshes[i].Draw(shader);
		}
	}
	~Model();
	babox getbox() {
		return box;
	}
	vector<Mesh> meshes;
	vector<Texture> textures_loaded;
private:
	babox box;//包围盒
	//模型数据
	string directory;
	//函数
	void loadMode(string path)
	{
		Assimp::Importer import;
		const aiScene *scene = import.ReadFile(path, aiProcess_Triangulate |  aiProcess_CalcTangentSpace);
		if (!scene||scene->mFlags&AI_SCENE_FLAGS_INCOMPLETE||!scene->mRootNode)
		{
			cout << "ERROR::ASSIMP::" << import.GetErrorString() << endl;
			return;
		}
		directory = path.substr(0, path.find_last_of('/'));
		processNode(scene->mRootNode, scene);
	}
	void processNode(aiNode *node, const aiScene * scene)
	{
		//处理节点所有网格(如果有的话）
		for (unsigned int i=0;i<node->mNumMeshes;i++)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			meshes.push_back(processMesh(mesh, scene));
		}
		//接下来对它的子节点重复执行
		for (unsigned int  i = 0; i < node->mNumChildren; i++)
		{
			processNode(node->mChildren[i], scene);
		}
	}
	Mesh processMesh(aiMesh *mesh, const aiScene* scene)
	{
		
		box.x.max = mesh->mVertices[0].x, box.x.min = mesh->mVertices[0].x;
		box.y.max = mesh->mVertices[0].y,box.y.min= mesh->mVertices[0].y;
		box.z.max = mesh->mVertices[0].z,box.z.min= mesh->mVertices[0].z;
		vector<Vertex> vertices;
		vector<unsigned int > indices;
		vector<Texture> textures;
		//处理坐标：顶点，法线，纹理坐标
		for (unsigned int i=0;i<mesh->mNumVertices;i++)
		{
			Vertex vertex;
			glm::vec3 vector;
			vector.x = mesh->mVertices[i].x;
			vector.y = mesh->mVertices[i].y;
			vector.z = mesh->mVertices[i].z;
			//x
			if (mesh->mVertices[i].x>box.x.max)
			{
				box.x.max = mesh->mVertices[i].x;
			}else if (mesh->mVertices[i].x<box.x.min)
			{
				box.x.min = mesh->mVertices[i].x;
			}
			//y
			if (mesh->mVertices[i].y > box.y.max)
			{
				box.y.max = mesh->mVertices[i].y;
			}
			else if (mesh->mVertices[i].y < box.y.min)
			{
				box.y.min = mesh->mVertices[i].y;
			}
			//z
			if (mesh->mVertices[i].z > box.z.max)
			{
				box.z.max = mesh->mVertices[i].z;
			}
			else if (mesh->mVertices[i].z < box.z.min)
			{
				box.z.min = mesh->mVertices[i].z;
			}
			vertex.Position = vector;
			vector.x = mesh->mNormals[i].x;
			vector.y = mesh->mNormals[i].y;
			vector.z = mesh->mNormals[i].z;
			vertex.Normal = vector;
			if (mesh->mTextureCoords[0])
			{
				glm::vec2 vec;
				vec.x = mesh->mTextureCoords[0][i].x;
				vec.y = mesh->mTextureCoords[0][i].y;
				vertex.TexCoords = vec;
			} 
			else
			{
				vertex.TexCoords = glm::vec2(0.0f, 0.0f);
			}
			vertices.push_back(vertex);
		}
		
		//处理索引
		for (unsigned int i=0;i<mesh->mNumFaces;i++)
		{
			aiFace face = mesh->mFaces[i];
			for (unsigned int j=0;j<face.mNumIndices;j++)
			{
				indices.push_back(face.mIndices[j]);
			}
		}
		//处理材质
		if (mesh->mMaterialIndex>=0)
		{
			aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
			vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
			textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
			// 2. specular maps
			vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
			textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
			//// 3. normal maps
			//std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
			//textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
			//// 4. height maps
			//std::vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
			//textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

		}
		//cout << "size= " << textures.size() << endl;
		return Mesh(vertices, indices, textures);
	}
	vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, string typeName)
	{
		vector<Texture> textures;
		for (unsigned int  i = 0; i < mat->GetTextureCount(type); i++)
		{
			aiString str;
			mat->GetTexture(type, i, &str);

			bool skip = false;
			for (unsigned int j = 0; j < textures_loaded.size(); j++)
			{
				if (std::strcmp(textures_loaded[j].path.data, str.C_Str()) == 0)
				{
					textures.push_back(textures_loaded[j]);
					skip = true;
					break;
				}
			}
			if (!skip)
			{
				Texture texture;
				texture.id = TextureFromFile(str.C_Str(), directory);
				texture.type = typeName;
				texture.path = str;
				//to be delete...
				//cout << texture.type << "  path= " << texture.path.C_Str() << " id= " << texture.id << endl;
				textures.push_back(texture);
				textures_loaded.push_back(texture);
			}
		}
		return textures;
	}
	
	
};


Model::~Model()
{
}
unsigned int TextureFromFile(const char* path, string	directory)
{
	string filename = string(path);
	filename = directory + "//" + filename;
	//生成纹理
	return LoadTex(filename);
}