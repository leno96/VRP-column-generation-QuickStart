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

//���ݵ�����ʼ��ͼ������ÿ�����㶼��������ÿ���ߵĳ����Ǻܴ��������
AdjMatrixDirNetwork::AdjMatrixDirNetwork( int vertexNum, int vertexMaxNum, int infinite)
{
	vexMaxNum = vertexMaxNum;
	vexNum = vertexNum;
	//����Ǽ���ֵ��Ĭ��ÿ�����㶼��������ÿ���ߵĳ����Ǻܴ��������
	infinity = infinite;
	arcNum = 0;
	vertexes = new int[vexMaxNum];
	tag = new VisitStatus[vexMaxNum];
	//��ά�ģ���0��ʼ������ÿ������������
	arcs = new double *[vexMaxNum];
	arcs_w = new double *[vexMaxNum];
	//ÿ�����������б�����
	for (int i = 0; i < vexMaxNum; i++) {
		arcs[i] = new double[vexMaxNum];
		arcs_w[i] = new double[vexMaxNum];
	}
		

		

	//��ʼ��
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

//����Ҫ�ı߸�Ȩ�أ��Ӷ��ñ߾��ǡ���Ч���ıߣ���¼��Ч�ߵ�����arcNum
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

//���ҵ���vex��������һ����Ч�ıߣ����س��ȶ�Ӧ�ĵ��š�
//��һ���������index��������ʼ�жϣ�Ĭ��indexӦ=0
int AdjMatrixDirNetwork::FirstAdjVex(int vex, int index)
{
	for (int i = index; i < vexNum; i++)
		if (arcs[vex][i] != infinity)
			return i;
	return -1;
}

//��vex2��һ���㿪ʼ�����������ҵ���һ�����������ģ����������ҵ���0-1���������vex2=1����2��ʼ��������
int AdjMatrixDirNetwork::NextAdjVex(int vex, int vex2)
{
	for (int i = vex2 + 1; i < vexNum; i++)
		if (arcs[vex][i] != infinity)
			return i;
	return -1;
}

//��2ά������ʽ�������е㵽���е�ıߵĳ������
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
