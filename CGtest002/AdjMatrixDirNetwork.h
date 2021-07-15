#pragma once
#include <iostream>
#include <limits>
#include "Problem.h"
#define DEFAULT_INFINITY INT_MAX
#define DEFAULT_SIZE 100
using namespace std;

//给了两种type
enum VisitStatus{VISIT,UNVISIT};

class AdjMatrixDirNetwork    //顶点为字符型
{
public:
	//AdjMatrixDirNetwork(int vertexMaxNum=DEFAULT_SIZE,int infinite=DEFAULT_INFINITY);  //默认构造函数，创建一个空图
	AdjMatrixDirNetwork(int vertexNum,int vertexMaxNum=DEFAULT_SIZE,int infinite=DEFAULT_INFINITY);
	~AdjMatrixDirNetwork();
	void InsertArc(int name_1, int name_2, int vex1,int vex2,double delta,double weight);   //输入合法，vex1与vex2小于vexNum，vex1!=vex2，不考虑平行边
	int GetElem(int vex);  
	int GetVexNum();
	int GetArcNum();
	double GetCost(int vex1, int vex2);
	int GetInfinity();
	int FirstAdjVex(int vex, int index);   //返回顶点vex的第一个邻接顶点的序号，若没有则返回-1
	int NextAdjVex(int vex,int vex2);   //返回顶点vex的相对于vex2的下一个邻接顶点的序号，若没有则返回-1
	void Display();
	VisitStatus GetTag(int vex);   //输入合法
	int SetTag(int vex,VisitStatus _tag);  //返回状态



private:
	int *vertexes;
	VisitStatus *tag;
	double **arcs;
	//专门记录载重
	double **arcs_w;
	int vexNum,vexMaxNum,arcNum;
	int infinity;

};
