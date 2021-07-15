#pragma once
#include <iostream>
#include <limits>
#include "Problem.h"
#define DEFAULT_INFINITY INT_MAX
#define DEFAULT_SIZE 100
using namespace std;

//��������type
enum VisitStatus{VISIT,UNVISIT};

class AdjMatrixDirNetwork    //����Ϊ�ַ���
{
public:
	//AdjMatrixDirNetwork(int vertexMaxNum=DEFAULT_SIZE,int infinite=DEFAULT_INFINITY);  //Ĭ�Ϲ��캯��������һ����ͼ
	AdjMatrixDirNetwork(int vertexNum,int vertexMaxNum=DEFAULT_SIZE,int infinite=DEFAULT_INFINITY);
	~AdjMatrixDirNetwork();
	void InsertArc(int name_1, int name_2, int vex1,int vex2,double delta,double weight);   //����Ϸ���vex1��vex2С��vexNum��vex1!=vex2��������ƽ�б�
	int GetElem(int vex);  
	int GetVexNum();
	int GetArcNum();
	double GetCost(int vex1, int vex2);
	int GetInfinity();
	int FirstAdjVex(int vex, int index);   //���ض���vex�ĵ�һ���ڽӶ������ţ���û���򷵻�-1
	int NextAdjVex(int vex,int vex2);   //���ض���vex�������vex2����һ���ڽӶ������ţ���û���򷵻�-1
	void Display();
	VisitStatus GetTag(int vex);   //����Ϸ�
	int SetTag(int vex,VisitStatus _tag);  //����״̬



private:
	int *vertexes;
	VisitStatus *tag;
	double **arcs;
	//ר�ż�¼����
	double **arcs_w;
	int vexNum,vexMaxNum,arcNum;
	int infinity;

};
