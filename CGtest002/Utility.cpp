#pragma once
#include "Utility.h"


clock_t Utility::m_start = 0;//clock_t start,finish;

string Utility::sLogFileName = "";
ofstream Utility::sLogOut;
bool Utility::sShowLog = false;//Ĭ�ϵ�һЩ�趨�����Ϊfalse�������

string Utility::sErrFileName = "";
ofstream Utility::sErrOut;//�����������Ϊ sErrOut
//ʱ�����޺�complete�Ͳ��ټ�����
double Utility::sTimeLimit = 400;// 7200;//

string Utility::sRouteFileName = "";
ofstream Utility::sRouteOut;
bool Utility::sShowRoute = false;


Utility::Utility()
{

}

Utility::~Utility()
{

}



 int Utility::RVND_MAX_NONIMPROVE = 0;
 int Utility::MAX_SHAKE_STRENGTH = 0;
 int Utility::PERTRUB_MAXTIME = 0;
 double Utility::NUM_DELET_A = 0;
 double Utility::NUM_DELET_B = 0;