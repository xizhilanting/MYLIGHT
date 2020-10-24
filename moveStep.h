#pragma once
//新版功能
#include <glm/glm/glm.hpp>
#include "glm/glm/gtc/matrix_transform.hpp"
#include "glm/glm/gtc/type_ptr.hpp"
#include <vector>
#include "CM_normal_Cal.h"
using namespace std;
float PI = 3.1416;
float g_rotate = 0;
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
	glm::vec3 dir{ glm::vec3(1.0f) };
	float stepRotate;
	float stepDis = 0;
	int curTranstimes = 0;
	int curRotatetimes = 0;
	float nowRotate = 0;
};
enum lookAtMode
{
	LOOKATPOINT,
	LOOKATDIR
};
enum line
{
	DONTMOVE,
	GOCIR

};
struct  checkBox
{
	babox box;
	lookAtMode lookMode= LOOKATDIR;//朝向模式
	glm::vec3 thDir{0, 0, 1.0f };//方向
	int thPoint = 0;//朝向人
	objInfo Info[100];
	int times = 3;
	int num = 0;//记录对象中管理的模型个数
	float R = 40.0f;//半径
};
class moveStep
{
private:
	checkBox thBox;
	line thLine = DONTMOVE;

public:

	moveStep()
	{
		thBox.times = 3;
	}
	~moveStep();
	//计算模长
	float calmoc(glm::vec3 tag) {
		return glm::sqrt(tag.x*tag.x + tag.y*tag.y + tag.z*tag.z);
	}
	
	//设置中心朝向并设置中心点
	void setCPoint(int i,float R=40.0f) {
		
		thLine = GOCIR;
		setDir(i);
		setTransCircle(i);
		setRotateCircle();
		thBox.R = R;
	}

	void addInstance(glm::mat4 curModel) {
		setInstance(curModel, curModel, thBox.num);
		thBox.num++;
	}
	void setInstance(glm::mat4 t_beginModel, glm::mat4 t_endModel, int i) {
		thBox.Info[i].beginModel = t_beginModel;
		thBox.Info[i].endModel = t_endModel;
		glm::vec3 disvec(t_endModel*glm::vec4(0, 0, 0, 1) - t_beginModel*glm::vec4(0, 0, 0, 1));//位移向量

		float dis = calmoc(disvec);//模长
		glm::vec3 dir = disvec / dis;//单位位移向量
		if (i==thBox.thPoint&&thBox.lookMode==LOOKATPOINT)
		{
			dir = glm::vec3(0, 0, 1);
			dis = 0;
			thBox.Info[i].dir = dir;
			//thBox.Info[i].nowRotate = vec2radi(dir);
		}
		else if (thBox.lookMode == LOOKATPOINT&&dis <= 0.01)//避免位置不动时角度计算错误的乱转
		{
			
			
		}
		else
		{
			if (dis <= 0.01)	dir = glm::vec3(0, 0, 1);
			thBox.Info[i].dir = dir;
			thBox.Info[i].nowRotate = vec2radi(dir);//在移动的时候更新朝向，最终为移动方向
		}
		thBox.Info[i].stepDis = dis / thBox.times;//每次移动的距离
		thBox.Info[i].curTranstimes = 0;
		thBox.Info[i].curRotatetimes = 0;

	}
	//设置圈圈位移//通过i传递中心然后计算各个点的位置
	void setTransCircle(unsigned int i)
	{
			int num = getNum();
		if(thBox.lookMode==LOOKATPOINT)
		{
			//double R = 40;
			float radi = 360.0 / (num - 1);
			int back = 0;
			int flag = 0;
			for (int j = 0; j < num; j++)
			{

				glm::mat4 cirModel = thBox.Info[i].beginModel;
				cirModel = glm::rotate(cirModel, glm::radians(radi*(j + flag)), glm::vec3(0, 1, 0));
				cirModel = glm::translate(cirModel, glm::vec3(thBox.R, 0, 0));
				float radi = getNowRotate(j);
				if (i != j)
				{

					setInstance(thBox.Info[j].beginModel, cirModel, j);
				}
				else
				{

					flag--;
					//glm::mat4 cirModel = getCurModel(i);
					//cirModel = glm::rotate(cirModel, glm::radians(radi*(j)), glm::vec3(0, 1, 0));
					//cirModel = glm::translate(cirModel, glm::vec3(thBox.R, 0, 0));
					setInstance(thBox.Info[j].beginModel, thBox.Info[j].beginModel, j);
				}
			}
		}
		else //if (thBox.lookMode==LOOKATDIR)
		{
			int k = 0;
			for (int j = 0; j < num; j++)
			{
				if (j==i)
				{
					continue;
				}
				glm::mat4 cirModel ;
				
				cirModel = glm::translate(thBox.Info[i].beginModel, glm::vec3(9.0f*(k % 5)-18, 0.0f, -7.0f*(1+k / 5)));
				//cirModel = glm::translate(cirModel, glm::vec3(9.0f*(j % 10), 0.0f, -7.0f*(j / 10)));
					setInstance(thBox.Info[j].beginModel, cirModel, j);
					k++;
			}
		}
	}
	
	
	//设置圈圈朝向和每次转向度数
	void setRotateCircle()
	{
		
		for (int i = 0; i < thBox.num; i++)
		{
			//thBox.Info[i].stepRotate = (vec2radi(getEndRotate(i)) - thBox.Info[i].nowRotate) / thBox.times;
			float rad = (getEndRotate(i) - thBox.Info[i].nowRotate);
			if (abs(rad) <= PI)
				rad = rad;
				//thBox.Info[i].stepRotate = rad / thBox.times;
			else
			{
				if (rad >= 0)
				{
					rad = rad - 2 * PI;
					//thBox.Info[i].stepRotate = (rad - 2 * PI) / thBox.times;
				}
				else
				{
					rad = 2 * PI + rad;
				//thBox.Info[i].stepRotate = (2 * PI + rad) / thBox.times;
				}
			}
			thBox.Info[i].stepRotate = rad / thBox.times;
			//printModelInfo(i);
			//cout << "id: " << i << endl;

		}
	}

	glm::mat4 getCurTransModel(int i)
	{
		if (thBox.Info[i].curTranstimes >= thBox.times)
		{
			return thBox.Info[i].beginModel;
			//return glm::rotate(thBox.Info[i].beginModel, vec2radi(-thBox.Info[i].dir), glm::vec3(0, 1, 0));
		}
		/*if (i==thBox.thPoint&&thBox.lookMode==LOOKATPOINT)
		{
			thBox.Info[i].curTranstimes++;
			return glm::rotate(thBox.Info[i].beginModel, thBox.Info[i].nowRotate, glm::vec3(0, 1, 0));
		}*/
		if (i == thBox.thPoint)
		{
			thBox.Info[i].curTranstimes++;
			return glm::rotate(thBox.Info[i].beginModel, thBox.Info[i].nowRotate, glm::vec3(0, 1, 0));
		}
			
		
		glm::mat4 tmp;
		tmp = glm::translate(thBox.Info[i].beginModel, thBox.Info[i].stepDis*thBox.Info[i].dir);//每次在当前位置向目标方向移动
		thBox.Info[i].beginModel = tmp;
		tmp = glm::rotate(tmp, thBox.Info[i].nowRotate, glm::vec3(0, 1, 0));
		
		thBox.Info[i].curTranstimes++;//当次数到达目标次数则停止
		
		return tmp;
	}
	glm::mat4 getCurRotateModel(int i)
	{
		
		
		if (thBox.Info[i].nowRotate>=2*PI)
		{
			thBox.Info[i].nowRotate = thBox.Info[i].nowRotate - 2*3.14159;
		}else if (thBox.Info[i].nowRotate <= -2 * PI)
		{
			thBox.Info[i].nowRotate = thBox.Info[i].nowRotate + 2 * 3.14159;
		}
		if (thBox.Info[i].curRotatetimes >= thBox.times)
		{
			return glm::rotate(glm::mat4(1.0f), (thBox.Info[i].nowRotate), glm::vec3(0, 1, 0));
		}
		/*if (i == thBox.thPoint&&thBox.lookMode == LOOKATPOINT)
		{
			thBox.Info[i].curRotatetimes++;
			thBox.Info[i].nowRotate = thBox.Info[i].nowRotate + thBox.Info[i].stepRotate;
			return glm::rotate(glm::mat4(1.0f), thBox.Info[i].nowRotate, glm::vec3(0, 1, 0));
		}*/
			
		glm::mat4 tmp;
		thBox.Info[i].nowRotate = thBox.Info[i].nowRotate + thBox.Info[i].stepRotate;
		tmp=glm::rotate(glm::mat4(1.0f), thBox.Info[i].nowRotate, glm::vec3(0, 1, 0));
		thBox.Info[i].curRotatetimes++;//当次数到达目标次数则停止
		return tmp;
	}
	glm::mat4 getCurModel(int i)
	{

		if (i >= 0 && i < thBox.num)
		{
			glm::mat4 model;
			
			if (thLine==GOCIR)
			{
				if (thBox.Info[i].curTranstimes >= thBox.times)
				{
					model = getCurRotateModel(i)*model;
				}
				model = getCurTransModel(i)*model;
				
			} 
			else if(thLine==DONTMOVE)
			{
				model = thBox.Info[i].beginModel;
			}
			//printModelInfo(i);
			return model;
		}
		return glm::mat4(1.0f);

	}
	//get model[i] endRotate
	float getEndRotate(int i)
	{
		if (thBox.lookMode == LOOKATPOINT)
		{
			glm::vec3 point = thBox.Info[thBox.thPoint].endModel*glm::vec4(0, 0, 0, 1);
			glm::vec3 thpos = thBox.Info[i].endModel*glm::vec4(0, 0, 0, 1);
			if (i == thBox.thPoint)
				return 0;
				/*return  glm::vec3(0, 0, 1);*/
			thpos = point - thpos;
			return vec2radi(thpos);
		}
		else// if(thBox.lookMode==LOOKATDIR)
		{
			//thBox.Info[i].nowRotate = 0;
			if (i==thBox.thPoint)
			{
				return 0;
			}
			//return thBox.Info[thBox.thPoint].nowRotate;
			return g_rotate;
		}
	}
	void printModelInfo(int i)
	{
		if (i >= 0&&i<thBox.num)
		{
			glm::vec3 trans = thBox.Info[i].beginModel*glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
			cout << "---------THE  NO." << i << "point= "<<thBox.thPoint<<"Info----------" << endl;
			cout << "pos= (" << trans.x << " " << trans.y << " " << trans.z << " )" << endl;
			cout << "nowRotate= " << thBox.Info[i].nowRotate << endl;
			cout << "STEPRotate= " << thBox.Info[i].stepRotate << endl;
			cout << "curTranstimes= " << thBox.Info[i].curTranstimes << endl;
			cout << "curRotatetimes= " << thBox.Info[i].curRotatetimes << endl;
			cout << "endRotate= " << getEndRotate(i) << endl;
			cout << "------------------------------------------" << endl;
		}
	}


	void setBox(double x_max, double y_max, double z_max,double x_min, double y_min, double z_min)
	{
		thBox.box.x.max = x_max;
		thBox.box.x.min = x_min;
		thBox.box.y.max = y_max;
		thBox.box.y.min = y_min;
		thBox.box.z.max = z_max;
		thBox.box.z.min = z_min;
	}
	//设置朝向
	void setDir(glm::vec3 lookat) {
		thBox.thDir = lookat;
	}
	//设置盯人
	void setDir(int i) {
		thBox.thPoint = i;
	}
	const babox getBox()
	{
		return thBox.box;
	}
	int getNum() {
		return thBox.num;
	}
	void changeLookMode()
	{

		if (thBox.lookMode == LOOKATDIR)
		{
			thBox.lookMode = LOOKATPOINT;

			cout << "中心模式" << endl;
		} 
		else if(thBox.lookMode == LOOKATPOINT)
		{
			thBox.lookMode = LOOKATDIR;
			cout << "队列模式" << endl;
		}
	}
	glm::vec3 getPos(int i) {
		return thBox.Info[i].beginModel*glm::vec4(0, 0, 0, 1);
	}
	float getNowRotate(int i) {
		return thBox.Info[i].nowRotate;
	}
	void moveCir(int x, int  z)
	{
		//if (thBox.lookMode==LOOKATPOINT)
		//{
		//	thBox.Info[thBox.thPoint].beginModel = glm::translate(thBox.Info[thBox.thPoint].beginModel, glm::vec3(x, 0, z));
		//	//thBox.Info[thBox.thPoint].nowRotate = vec2radi(glm::vec3(x, 0, z));
		//	setCPoint(thBox.thPoint);

		//}
		//else
		{
			glm::mat4 tmp;
			/*float throtate;
			static float oldRotate = thBox.Info[thBox.thPoint].nowRotate;
			if (oldRotate== thBox.Info[thBox.thPoint].nowRotate)
			{
				throtate = 0;
			}
			else
			{
				oldRotate = thBox.Info[thBox.thPoint].nowRotate;
				throtate = oldRotate;
			}*/
			static bool left = false, right = false;
			int ileft = 0, iright = 0;
			tmp= glm::translate(thBox.Info[thBox.thPoint].beginModel, glm::vec3(0, 0, z));
			if (x>0)
			{
				ileft++;
				if (left)
				{
					thBox.Info[thBox.thPoint].beginModel = tmp;
				} 
				else if(right)
				{
					thBox.Info[thBox.thPoint].beginModel = glm::rotate(tmp, iright*0.1f, glm::vec3(0, 1, 0));

				}
				else
				{
					thBox.Info[thBox.thPoint].beginModel = glm::rotate(tmp, 0.1f, glm::vec3(0, 1, 0));
				}
				left = true;
				right = false;
			} 
			else if(x < 0)
			{
				iright++;
				if (right)
				{
					thBox.Info[thBox.thPoint].beginModel = tmp;
				}
				else if (left)
				{
					thBox.Info[thBox.thPoint].beginModel = glm::rotate(tmp, -ileft*0.1f, glm::vec3(0, 1, 0));
				}
				else
				{
					thBox.Info[thBox.thPoint].beginModel = glm::rotate(tmp, -0.1f, glm::vec3(0, 1, 0));
				}
				right = true;
				left = false;
			}
			else
			{
				if (left)
				{
					thBox.Info[thBox.thPoint].beginModel = glm::rotate(tmp, -ileft*0.1f, glm::vec3(0, 1, 0));
				} 
				else if(right)
				{
					thBox.Info[thBox.thPoint].beginModel = glm::rotate(tmp, iright*0.1f, glm::vec3(0, 1, 0));
				}
				else
					thBox.Info[thBox.thPoint].beginModel = tmp;
				right = false;
				left = false;
			}
			//thBox.Info[thBox.thPoint].beginModel = glm::rotate(tmp, vec2radi(glm::vec3(x, 0, 0))*0.01f, glm::vec3(0, 1, 0));
			//thBox.Info[thBox.thPoint].beginModel = glm::translate(thBox.Info[thBox.thPoint].beginModel, glm::vec3(x, 0, z));
			//thBox.Info[thBox.thPoint].nowRotate = vec2radi(glm::vec3(x, 0, z));
			thBox.Info[thBox.thPoint].curRotatetimes = 0;
			thBox.Info[thBox.thPoint].curTranstimes = 0;
			static int nright = 0, nleft = 0;
			nright += iright;
			nleft += ileft;
			//thBox.Info[thBox.thPoint].nowRotate 
			g_rotate= -(nright - nleft)*0.1;
			setCPoint(thBox.thPoint);
			
		}
	}
	void getLeadre(glm::vec3 &pos,float &yaw)
	{
		pos=thBox.Info[thBox.thPoint].beginModel*glm::vec4(0, 0, 0, 1);
		yaw = acos(thBox.Info[thBox.thPoint].beginModel[0][0]);
	}

};


moveStep::~moveStep()
{
}
