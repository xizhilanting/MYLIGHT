#pragma once
#include <glm/glm/glm.hpp>
#include "glm/glm/gtc/matrix_transform.hpp"
#include "glm/glm/gtc/type_ptr.hpp"
#include "CM_normal_Cal.h"
//人
class People
{
public:
	//初始化需要提供两个vec3 也可不提供（使用默认）
	People(glm::vec3 th_pos = glm::vec3(sID*13, 0, 0),glm::vec3 th_dir=glm::vec3(0,0,-1)) :cur_pos(th_pos), ID(sID++), cur_dir(th_dir) {	}
	~People();
	//设置新位置
	void setpos(glm::vec3 th_pos)
	{
		tar_pos = th_pos;

	}
	void setdir(glm::vec3 th_dir)
	{
		tar_dir = th_dir;
	}
	void settar(glm::vec3 th_pos, glm::vec3 th_dir)
	{
		setpos(th_pos);
		setdir(th_dir);
	}

	//获取
	glm::vec3 get_now_pos() {
		if (!need_to_go)	return cur_pos;
	}
	glm::vec3 get_now_dir() {
		if (!need_to_go)		return cur_dir;
	}
	glm::mat4 get_now_model() {
		glm::vec3 pos = get_now_pos();
		glm::vec3 dir = get_now_dir();
		glm::mat4 model_tmp = glm::rotate(glm::mat4(1), vec2radi(dir), glm::vec3(0, 1, 0));
		glm::mat4 model_tmp = glm::translate(model_tmp, pos);
		return model_tmp;
	}
private:
	glm::vec3	cur_pos;
	glm::vec3	cur_dir;
	glm::vec3	tar_pos;
	glm::vec3	tar_dir;
	bool need_to_go = false;
	int ID;
	static int sID;
};
int People::sID = 0;

People::~People()
{
}
