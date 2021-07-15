#pragma once
#include <iostream>
#include <limits>
#include "AdjMatrixDirNetwork.h"



//AdjMatrixDirNetwork::AdjMatrixDirNetwork(int vertexMaxNum, int infinite)
//{
//	vexMaxNum = vertexMaxNum;
//	vexNum = 0;
//	arcNum = 0;
//	infinity = infinite;
//	vertexes = new int[vexMaxNum];
//	tag = new VisitStatus[vexMaxNum];
//	arcs = new double *[vexMaxNum];
//	for (int i = 0; i < vexMaxNum; i++)
//		arcs[i] = new double[vexMaxNum];
//	arcs_w = new double *[vexMaxNum];
//	for (int i = 0; i < vexMaxNum; i++)
//		arcs_w[i] = new double[vexMaxNum];
//}

//根据点数初始化图，假设每两个点都相连，且每个边的长度是很大的正数。
AdjMatrixDirNetwork::AdjMatrixDirNetwork( int vertexNum, int vertexMaxNum, int infinite)
{
	vexMaxNum = vertexMaxNum;
	vexNum = vertexNum;
	//这个是极大值，默认每两个点都相连，且每个边的长度是很大的正数。
	infinity = infinite;
	arcNum = 0;
	vertexes = new int[vexMaxNum];
	tag = new VisitStatus[vexMaxNum];
	//二维的，从0开始计数，每个点流出到哪
	arcs = new double *[vexMaxNum];
	arcs_w = new double *[vexMaxNum];
	//每个点最多和所有边相连
	for (int i = 0; i < vexMaxNum; i++) {
		arcs[i] = new double[vexMaxNum];
		arcs_w[i] = new double[vexMaxNum];
	}
		

		

	//初始化
	for (int i = 0; i < vexMaxNum; i++)
	{
		
		tag[i] = UNVISIT;
		for (int j = 0; j < vexMaxNum; j++) {
			arcs[i][j] = infinity;
			arcs_w[i][j] = infinity;
		}
			
	}
}

AdjMatrixDirNetwork::~AdjMatrixDirNetwork()
{
	if (vertexes != NULL)
		delete[] vertexes;
	if (tag != NULL)
		delete[] tag;
	if (arcs != NULL)
	{
		for (int i = 0; i < vexNum; i++) {
			delete[] arcs[i];
			delete[] arcs_w[i];
		}
			
		delete[] arcs;
		delete[] arcs_w;
	}
}

//给需要的边赋权重，从而该边就是“有效”的边；记录有效边的数量arcNum
void AdjMatrixDirNetwork::InsertArc(int name_1,int name_2,int vex1, int vex2, double delta, double weight)
{
	arcs[vex1][vex2] = delta;
	arcs_w[vex1][vex2] = weight;
	vertexes[vex1] = name_1;
	vertexes[vex2] = name_2;
	arcNum++;
}

int AdjMatrixDirNetwork::GetElem(int vex)
{
	return vertexes[vex];
}

int AdjMatrixDirNetwork::GetVexNum()
{
	return vexNum;
}

int AdjMatrixDirNetwork::GetArcNum()
{
	return arcNum;
}

double AdjMatrixDirNetwork::GetCost(int vex1, int vex2)
{
	return arcs[vex1][vex2];
}

int AdjMatrixDirNetwork::GetInfinity()
{
	return infinity;
}

//【找到从vex出发，第一个有效的边，返回出度对应的点编号】
//第一个到达点是index，从他开始判断，默认index应=0
int AdjMatrixDirNetwork::FirstAdjVex(int vex, int index)
{
	for (int i = index; i < vexNum; i++)
		if (arcs[vex][i] != infinity)
			return i;
	return -1;
}

//从vex2下一个点开始往后搜索，找到第一个。配合上面的，比如上面找到了0-1，那这次令vex2=1，从2开始往后搜索
int AdjMatrixDirNetwork::NextAdjVex(int vex, int vex2)
{
	for (int i = vex2 + 1; i < vexNum; i++)
		if (arcs[vex][i] != infinity)
			return i;
	return -1;
}

//按2维矩阵形式，把所有点到所有点的边的长度输出
void AdjMatrixDirNetwork::Display()
{
	if (vexNum != 0)
	{
		for (int i = 0; i < vexNum; i++)
		{
			cout << "[ ";
			for (int j = 0; j < vexNum; j++)
				cout << arcs[i][j] << " ";
			cout << "]" << endl;
		}
	}
}

VisitStatus AdjMatrixDirNetwork::GetTag(int vex)
{
	return tag[vex];
}

int AdjMatrixDirNetwork::SetTag(int vex, VisitStatus _tag)
{
	if (vex >= vexNum)
		return 0;
	tag[vex] = _tag;
	return 1;
}
