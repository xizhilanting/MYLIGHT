#pragma once
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <map>
#include "glm/glm/glm.hpp"
#include "glm/glm/gtc/matrix_transform.hpp"
#include "glm/glm/gtc/type_ptr.hpp"
using namespace std;

stringstream& operator>> (stringstream& op, glm::vec3& oop);
stringstream& operator>> (stringstream& op, glm::mat4& oop);
string to_string(glm::vec3 vec3);
string to_string(glm::mat4 mat4);
class conff
{
public:
	conff();
	~conff();
	//int
	template<typename T>
	bool getPara(string name, T &para)
	{
		if (content.count(name))
		{
			stringstream stm(content[name]);
			stm >> para;
			return true;
		}
		return false;
	}

	template<typename T>
	void setPare(string name, const T& para)
	{
		content[name] = to_string(para);
		
	}
	template<typename T>
	void addPare(string name, const T& para)
	{
		string tmp = to_string(para)+" "+ content[name];
		content[name] = tmp;
	}
private:
	map<string, string> content;	//用于存储内容

};

conff::conff()
{
	ifstream readFile;
	readFile.open("config");
	if (!readFile.is_open())
	{
		cout << "Error open" << endl;
	}
	else
	{
		string str_tmp;
		while (getline(readFile, str_tmp))
		{
			if (true)	//用于判断无效行
			{
				//前后清空空白
				if (str_tmp.find(' ') < str_tmp.size())//如果有内容
				{
					content[str_tmp.substr(0, str_tmp.find(' '))] = str_tmp.substr(str_tmp.find(' ') + 1);
				}
			}
		}
	}
	readFile.close();
	//清空
	fstream fs("config", fstream::out | ios_base::trunc);
	fs.close();
}

conff::~conff()
{
	ofstream writeFile;
	writeFile.open("config");
	if (!writeFile.is_open())
	{
		cout << "Error write open" << endl;
	}
	else
	{
		string str_tmp;
		for (auto i : content)
		{
			writeFile << i.first << " " << i.second << endl;
		}
	}
	writeFile.close();
	content.clear();
}


stringstream& operator>> (stringstream& op, glm::vec3& oop)
{
	op >> oop.x >> oop.y >> oop.z;
	return op;

}



stringstream& operator>> (stringstream& op, glm::mat4& oop)
{
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			op >> oop[j][i];
	return op;

}

string to_string(glm::vec3 vec3)
{
	return string(to_string(vec3.x) + " " + to_string(vec3.y) + " " + to_string(vec3.z));
}



string to_string(glm::mat4 mat4)
{
	string tmp;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			tmp += to_string(mat4[j][i]) + " ";
		}
	}
	return tmp;
}