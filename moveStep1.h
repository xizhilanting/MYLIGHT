#pragma once
#include <glm/glm/glm.hpp>
#include "glm/glm/gtc/matrix_transform.hpp"
#include "glm/glm/gtc/type_ptr.hpp"
#include <vector>
using namespace std;
struct comp
{
	double max;
	double min;
};
struct babox
{
	comp x;
	comp y;
	comp z;
};
struct objInfo
{

	glm::mat4 beginModel = glm::mat4(1.0f);
	glm::mat4 endModel = glm::mat4(1.0f);
	glm::mat4 beginNrl = glm::rotate(glm::mat4(1.0f),0.0f,glm::vec3(0,1,0));
	glm::vec3 dir{glm::vec3(1.0f)};
	float stepRotate;
	float stepDis=0;
	int curTranstimes = 0;
	float nowRotate = 0;
};
struct  checkBox
{
	babox box;
	objInfo Info[100];
	int times=10;
	int num = 0;
};
class moveStep
{
public:
	moveStep();
	~moveStep();
	float calmoc(glm::vec3 tag) {
		return glm::sqrt(tag.x*tag.x + tag.y*tag.y + tag.z*tag.z);
	}
	void addInstance(glm::mat4 curModel,float radi) {
		setInstance(curModel, curModel, radi,thBox.num);
		thBox.num++;
	}
	void setInstance(glm::mat4 t_beginModel, glm::mat4 t_endModel, float radi, int i) {
		thBox.Info[i].beginModel = t_beginModel;
		thBox.Info[i].endModel = t_endModel;
		glm::vec3 disvec(t_endModel*glm::vec4(0, 0, 0, 1) - t_beginModel*glm::vec4(0, 0, 0, 1));//位移向量
		float dis = calmoc(disvec);//模长
		glm::vec3 dir = disvec/dis;//单位位移向量
		thBox.Info[i].stepRotate = radi/ thBox.times;
		thBox.Info[i].dir = dir;
		thBox.Info[i].stepDis = dis/ thBox.times;//每次移动的距离
		thBox.Info[i].curTranstimes = 0;
		
	}
	glm::mat4 getCurModel(int i)
	{
		
		if (i>=0&&i<thBox.num)
		{
			if (thBox.Info[i].curTranstimes>=thBox.times|| thBox.Info[i].stepDis==0||thBox.Info[i].stepRotate==0)
			{
				return thBox.Info[i].beginModel;
				//return thBox.Info[i].beginModel*thBox.Info[i].beginNrl;
			}
			
			//	cout << thBox.Info[i].curtimes << endl;
			glm::mat4 tmp = glm::translate(thBox.Info[i].beginModel, thBox.Info[i].stepDis*thBox.Info[i].dir);//每次在当前位置向目标方向移动
			thBox.Info[i].beginModel = tmp;
			thBox.Info[i].curTranstimes++;//当次数到达目标次数则停止
			thBox.Info[i].beginNrl = glm::rotate(thBox.Info[i].beginNrl, glm::radians(thBox.Info[i].stepRotate), glm::vec3(0, 1, 0));
			thBox.Info[i].nowRotate += thBox.Info[i].stepRotate;
			return thBox.Info[i].beginModel;

		}
		return glm::mat4(1.0f);

	}
	void setBox(double x_max, double y_max, double z_max,
		double x_min, double y_min, double z_min)
	{
		thBox.box.x.max = x_max;
		thBox.box.x.min = x_min;
		thBox.box.y.max = y_max;
		thBox.box.y.min = y_min;
		thBox.box.z.max = z_max;
		thBox.box.z.min = z_min;
	}
	const babox getBox()
	{
		return thBox.box;
	}
	int getNum() {
		return thBox.num;
	}
	glm::mat4 getNrl(int i) {
		return thBox.Info[i].beginNrl;
	}
	glm::vec3 getPos(int i) {
		return thBox.Info[i].beginModel*glm::vec4(0, 0, 0, 1);
	}
	float getNowRotate(int i) {
		return thBox.Info[i].nowRotate;
	}
private:
	
	checkBox thBox;

};

moveStep::moveStep()
{
	thBox.times = 10;
}

moveStep::~moveStep()
{
}
