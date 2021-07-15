
#pragma once
#include <string>
#include <vector>


#ifndef NULL
#define NULL   ((void *) 0)
#endif





class Conf//这些参数的值在不同cpp文件中具体指定
{
public:
	//fixed parameters
	static int m_saa_number;
	static int m_test_number;
//	static double deviation;
	static double minCv;
	static double maxCv;
	static double speed;
	static double budget_coef;
	static double m_budget;
	static double eps;
	static double eps_2;

	static int reg_k;
	static double big_number;
	static double penalty;

	static double cvar_ltail;	// the left tail probability, i.e., \gamma in our paper
	static double radius;
	static double wconstant;

	static int cplex_time_lim;
	static int cplex_mem_lim;
	static int cplex_thread_lim;
	static double RCI_MinCAPViolation;

	static double run_time;
	static double best_time;
	static int nPathOptCut;
	static int nPathFesCut;

	static int nNodeSolomon;
	static int nVehicleSolomon;
	static std::string opt_file;


	static std::vector<double> obj_record;

	static double std_ratio;
};

