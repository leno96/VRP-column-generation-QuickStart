#pragma once
#include "Conf.h"
#include <string>
#include "Utility.h"

int Conf::m_saa_number = 200;
int Conf::m_test_number = 10000;
double Conf::run_time = 0;
double Conf::best_time = 0;
double Conf::minCv = 0.1;
double Conf::maxCv = 0.5;
double Conf::speed = 1;
double Conf::budget_coef = 1.05;
double Conf::m_budget = Conf::big_number;
double Conf::eps = 0.02;// 1;//1e-3;
double Conf::eps_2 = 0.02;//1e-3;
int Conf::reg_k = 5;
double Conf::big_number = 1e7;
double Conf::penalty = 1e7;

std::vector<double> Conf::obj_record;

