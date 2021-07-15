#pragma once
#include "Utility.h"


clock_t Utility::m_start = 0;//clock_t start,finish;

string Utility::sLogFileName = "";
ofstream Utility::sLogOut;
bool Utility::sShowLog = false;//默认的一些设定，如果为false，则不输出

string Utility::sErrFileName = "";
ofstream Utility::sErrOut;//定义输出符号为 sErrOut
//时间上限后，complete就不再计算了
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