/* ==================================================================
							   INCLUDES
   ================================================================== */
#pragma once
#include "ilcplex\cplex.h"		// include files for CPLEX
#include "Problem.h"	
#include "Feasibility.h"	
#include <vector>				// so we can use a vector
#include <chrono>				// so we can use timers
#include <stdexcept>			// so we can throw and catch exceptions
#include <fstream>				// to enable input from files and output to files
#include <iostream>				// to write output to the screen
#include <memory>				// so we can use unique_ptr for arrays
#include <string>				// so we can use strings

using namespace std;



/* ==================================================================
							 GLOBAL VARIABLES
   ================================================================== */

   // ================ PROBLEM DATA ================ 

   // The number of lectures
int nb_lectures;

// The number of timeslots
int nb_timeslots;

// The number of rooms
int nb_rooms;

// The number of professors
int nb_professors;

// Indicates which professors teach which lectures
vector<int> professors_lecture;	// professor_lecture.at(p * nb_lectures + l) == 1 if professor p teacher lecture l, 0 otherwise

// Indicates the cost of assigning a lecture to a given timeslot
vector<int> cost_lecture_timeslot; // cost_lecture_timeslot.at(l * nb_timeslots + t) == cost of assigning lecture l to timeslot t

vector<Route> route_list;

// ================ CPLEX VARIABLES ================ 

// A pointer to the CPLEX environment
CPXENVptr env = nullptr;

// A pointer to the master problem
CPXLPptr master_problem = nullptr;

// A pointer to the pricing problem
CPXLPptr pricing_problem = nullptr;

// Char buffer for CPLEX error messages
char error_text[CPXMESSAGEBUFSIZE];

// The solution of the master problem from CPLEX
unique_ptr<double[]> solution_masterproblem;

// Indices of the variables for which we change the coefficients in the pricing problem
unique_ptr<int[]> indices_variables_pricingproblem;

// Coefficients of the variables for which we change the coefficients in the pricing problem
unique_ptr<double[]> new_coefficients_pricingproblem;

// The dual prices from the master problem
unique_ptr<double[]> dual_prices;

// Objective value of the master problem
double objective_value_masterproblem;

// Objective value of the pricing problem
double objective_value_pricingproblem;

// Objective function coefficient in master of newly found column
double cost_new_column;

// Values in constraint matrix master of newly found column
vector<int> new_column_a;

// The number of columns added
int nb_columns_added;

// The computation time in seconds
double computation_time;





/* ==================================================================
						READ PROBLEM DATA
   ================================================================== */

void read_problem_data_from_file(const string& file_name)
{
	ifstream file;
	file.open(file_name);
	if (!file.is_open())
		throw runtime_error("Couldn't open the input file name \"" + file_name + "\"");

	// Read in the basic data
	file >> nb_lectures;
	file >> nb_timeslots;
	file >> nb_rooms;
	file >> nb_professors;


	//用vector读入
	// Read in the professor_lecture matrix
	for (int p = 0; p < nb_professors; ++p)
	{
		for (int l = 0; l < nb_lectures; ++l)
		{
			int k;
			file >> k;
			professors_lecture.push_back(k);

		}
	}

	// Read in the cost_lecture_timeslot matrix
	for (int l = 0; l < nb_lectures; ++l)
	{
		for (int t = 0; t < nb_timeslots; ++t)
		{
			int k;
			file >> k;
			cost_lecture_timeslot.push_back(k);
		}
	}
}





/* ==================================================================
							INITIALIZE CPLEX
   ================================================================== */

void initialize_cplex()
{
	int status = 0;

	// Open the cplex environment
	env = CPXopenCPLEX(&status);
	if (status != 0)
	{
		CPXgeterrorstring(env, status, error_text);
		throw runtime_error("Error in function initialize_cplex(). \nCouldn't open CPLEX. \nReason: " + string(error_text));
	}

	// Set the output to screen on/off
	//【参数设置&&不输出到屏幕】
	status = CPXsetintparam(env, CPX_PARAM_SCRIND, CPX_OFF);
	if (status != 0)
	{
		CPXgeterrorstring(env, status, error_text);
		throw runtime_error("Error in function initialize_cplex(). \nCouldn't set the output to screen off. \nReason: " + string(error_text));
	}
}




/* ==================================================================
						   BUILD MASTER PROBLEM
   ================================================================== */

void build_master_problem(const Problem &p)
{
	int status = 0;
	double obj[1];						// Objective function coefficient of variables
	double lb[1];						// Lower bounds of variables
	double ub[1];						// Upper bounds of variables
	double rhs[1];						// Right-hand side of constraints
	char *colname[1];					// Variable names//列是变量，行是约束
	char *rowname[1];					// Constraint names
	char sense[1];						// Sign of constraint ('L' for <=, 'E' for ==, 'G' for >=)
	int matbeg[1];						// Begin position of the constraint (always 0)
	unique_ptr<int[]> matind;		// Position of each element in constraint matrix
	unique_ptr<double[]> matval;	// Value of each element in constraint matrix
	int nonzeros = 0;					// To calculate number of nonzero coefficients in each constraint


	// Create arrays
	//make_unique就是new（智慧，不用操心delete）
	matind = make_unique<int[]>(2000);
	matval = make_unique<double[]>(2000);

	// Create the problem
	//CPXcreateprob creates a CPLEX problem object in the CPLEX environment
	master_problem = CPXcreateprob(env, &status, "master_problem");
	if (status != 0)
	{
		CPXgeterrorstring(env, status, error_text);
		throw runtime_error("Error in function build_master_problem(). \nCouldn't create the CPLEX problem. \nReason: " + string(error_text));
	}

	// Problem is minimization
	CPXchgobjsen(env, master_problem, CPX_MIN);
	if (status != 0)
	{
		CPXgeterrorstring(env, status, error_text);
		throw runtime_error("Error in function build_master_problem(). \nCouldn't set the problem type to minimization. \nReason: " + string(error_text));
	}

	// VARIABLES
	// Add the z_tj variables
	// Here we add a super column for every timeslot
	//先添加custnumber个客户一一对应的路径作为初始解
	for (int t = 0; t <p.m_customer_number ; ++t)
	{
		string name = "X" + to_string(t + 1);
		//x1代表方案1

		//这两行刚好定义名字
		colname[0] = const_cast<char*>(name.c_str());

		obj[0] = p.GetDistance(p.m_customer_number + 1, t + 1) + p.GetDistance(t + 1, p.m_customer_number + 1); // cost of the super column: very high objective function coefficient

		lb[0] = 0;
		ub[0] = 1;
		//【系统地】地定义要加入的变量，包括变量名、变量数、上下界、目标函数中的系数
		//1的位置：被加入的新的变量数。An integer that specifies the number of new variables being added to the problem object.
		//obj的位置：目标函数里该变量的系数。An 【array】 of length ccnt containing the objective function coefficients of the new variables.
		//null的位置：是变量的类型，如果是NULL默认连续型。This array may be NULL, in which case the new variables are created as continuous type. If this array is not NULL, then CPLEX interprets the problem as a MIP
		//colname是列名的数组，也就是变量名们
		//对于lp中的bounds和目标函数
		//newcols是定义初始变量；而addcols是添加变量
		status = CPXnewcols(env, master_problem, 1, obj, lb, ub, NULL, colname); // Generate columns (the variables) 
		if (status != 0)
		{
			CPXgeterrorstring(env, status, error_text);
			throw runtime_error("Error in function build_master_problem(). \nCouldn't add variable " + name + ". \nReason: " + string(error_text));
		}
	}


	// CONSTRAINTS
		//一个约束集一个约束集地添加。专门给哪些对于所有i如何如何的约束用。
	// Constraint set 1
	for (int l = 0; l < p.m_customer_number; ++l) // forall(l in L)
	{
		sense[0] = 'E';	// constraint is '=='
		rhs[0] = 1;		// right-hand side of constraint equals 1

		string name = "node_" + to_string(l + 1) + "_has to be one contained!";
		rowname[0] = const_cast<char*>(name.c_str());

		matbeg[0] = 0;	// always 0
		nonzeros = 0;	// number of coefficients in constraint different from 0



		//matind是下面CPXaddrows要添加的约束所涉及到的变量，【在上面定义新变量矩阵中的位置】，比如这里
		//matind[nonzeros] = 0是指调用的是上面定义的第一个变量，也就是z_super_0；matbeg是查询的起点，一般是从定义的最开头开始查找，所以是0
		matind[nonzeros] = l;	// z_super_t is at position t in the constraint matrix
		//系数为1，因为初始解每行对应1个方案，包含1个客户点
		matval[nonzeros] = 1;	// z_super_0 has a coefficient of 1 in this constraint
		++nonzeros;		// increase variable nonzeros by 1, as we have a variable with nonzero coefficient in the constraint
		
		// add the constraint to CPLEX
		//0的位置是该约束里新变量的个数
		//1的位置是新加入的约束数量
		//nonzeros：指定要添加到约束矩阵的非零约束系数的数量，和matind元素数量一致
		// matbeg, matind.get(), matval.get() define the rows to be added
		//matbeg(beginning of the matrix), matcnt(count of the matrix), matind(indices of the matrix), and matval(values of the matrix),
		//该约束里新变量的个数为0，表示这个指令只是对之前定义过的变量进行操作（具体调用哪个变量是matind去选择，系数由matval确定）
		//null的位置是新增变量的名字，这里没有临时新添加变量到约束里，所以是null
		status = CPXaddrows(env, master_problem, 0, 1, nonzeros, rhs, sense, matbeg, matind.get(), matval.get(), NULL, rowname);
		if (status != 0)
		{
			CPXgeterrorstring(env, status, error_text);
			throw runtime_error("Error in function build_master_problem(). \nCouldn't add constraint " + name + ". \nReason: " + string(error_text));
		}
	}

	//先不考虑车辆数约束
	//// Constraint set 2: exactly one column selected for every timeslot
	//for (int t = 0; t < nb_timeslots; ++t) // forall(t in T)
	//{
	//	sense[0] = 'E';	// constraint is '=='
	//	rhs[0] = 1;		// right-hand side of constraint equals 1

	//	string name = "Exactly_one_column_selected_for_timeslot_" + to_string(t + 1);
	//	rowname[0] = const_cast<char*>(name.c_str());

	//	matbeg[0] = 0;	// always 0
	//	nonzeros = 0;	// number of coefficients in constraint different from 0
	//	//该变量在变量矩阵中的位置是t，前面系数是1
	//	matind[nonzeros] = t;	// z_super_t is at position t in the constraint matrix
	//	matval[nonzeros] = 1;	// z_super_0 has a coefficient of 1 in this constraint
	//	++nonzeros;		// increase variable nonzeros by 1, as we have a variable with nonzero coefficient in the constraint

	//	// add the constraint to CPLEX
	//	//1表示添加一个约束，这里是一个一个添加
	//	status = CPXaddrows(env, master_problem, 0, 1, nonzeros, rhs, sense, matbeg, matind.get(), matval.get(), NULL, rowname);
	//	if (status != 0)
	//	{
	//		CPXgeterrorstring(env, status, error_text);
	//		throw runtime_error("Error in function build_master_problem(). \nCouldn't add constraint " + name + ". \nReason: " + string(error_text));
	//	}
	//}


	// Write the problem to an LP-file, so that we can check whether it is correct
	status = CPXwriteprob(env, master_problem, "master_problem.lp", NULL);
	if (status != 0)
	{
		CPXgeterrorstring(env, status, error_text);
		throw runtime_error("Error in function build_master_problem(). \nFailed to write the problem to a file. \nReason: " + string(error_text));
	}


	// Allocate memory for dual variables
	indices_variables_pricingproblem = make_unique<int[]>(p.m_customer_number);

	for (int i = 0; i < p.m_customer_number; ++i)
		indices_variables_pricingproblem[i] = i;

	new_coefficients_pricingproblem = make_unique<double[]>(p.m_customer_number);

	//【这个数量和约束数量相一致】
	dual_prices = make_unique<double[]>(p.m_customer_number);

	//new_column_a.reserve(p.m_customer_number);
	for (int l = 0; l < p.m_customer_number; ++l)
		new_column_a.push_back(0);
}





/* ==================================================================
						 BUILD PRICING PROBLEM
   ================================================================== */

   // We build a single pricing problem instead of one pricing problem for all timeslots.
   // We will change the relevant coefficients each time we solve the pricing problem 
   // for a different timeslot.

//这个不需要
void build_pricing_problem()
{

	
	int status = 0;
	double obj[1];						// Objective function coefficient of variables
	double lb[1];						// Lower bounds of variables
	double ub[1];						// Upper bounds of variables
	double rhs[1];						// Right-hand side of constraints
	char *colname[1];					// Variable names
	char *rowname[1];					// Constraint names
	char sense[1];						// Sign of constraint ('L' for <=, 'E' for ==, 'G' for >=)
	int matbeg[1];						// Begin position of the constraint (always 0)
	unique_ptr<int[]> matind;		// Position of each element in constraint matrix
	unique_ptr<double[]> matval;	// Value of each element in constraint matrix
	char type[1];						// Type of variable (integer 'I', binary 'B', fractional 'C')
	int nonzeros = 0;					// To calculate number of nonzero coefficients in each constraint


	// Create arrays
	matind = make_unique<int[]>(2000);
	matval = make_unique<double[]>(2000);

	// Create the problem
	pricing_problem = CPXcreateprob(env, &status, "pricing_problem");
	if (status != 0)
	{
		CPXgeterrorstring(env, status, error_text);
		throw runtime_error("Error in function build_pricing_problem(). \nCouldn't create the CPLEX problem. \nReason: " + string(error_text));
	}

	// Problem is minimization
	CPXchgobjsen(env, pricing_problem, CPX_MIN);
	if (status != 0)
	{
		CPXgeterrorstring(env, status, error_text);
		throw runtime_error("Error in function build_pricing_problem(). \nCouldn't set the problem type to minimization. \nReason: " + string(error_text));
	}

	// VARIABLES
	// Add the a_lr variables
	for (int l = 0; l < nb_lectures; ++l)
	{
		for (int r = 0; r < nb_rooms; ++r)
		{
			string name = "a_" + to_string(l + 1) + "_" + to_string(r + 1);
			colname[0] = const_cast<char*>(name.c_str());

			obj[0] = 0; // objective function coefficient of zero (this will be changed to the value of (c_lt - mu_l) when solving the problem

			lb[0] = 0;
			ub[0] = 1;
			type[0] = 'B'; // binary variable
			//这里是求解子问题；先定义子问题的变量，这里嵌套在循环里，每轮循环添加一个新变量
			status = CPXnewcols(env, pricing_problem, 1, obj, lb, ub, type, colname); // Generate columns (the variables) 
			if (status != 0)
			{
				CPXgeterrorstring(env, status, error_text);
				throw runtime_error("Error in function build_pricing_problem(). \nCouldn't add variable " + name + ". \nReason: " + string(error_text));
			}
		}
	}

	// CONSTRAINTS
	// Constraint set 1: at most one lecture per room 
	for (int r = 0; r < nb_rooms; ++r) // forall(r in R)
	{
		sense[0] = 'L';	// constraint is '<='
		rhs[0] = 1;		// right-hand side of constraint equals 1

		string name = "At_most_one_lecture_in_room_" + to_string(r + 1);
		rowname[0] = const_cast<char*>(name.c_str());

		matbeg[0] = 0;	// always 0
		nonzeros = 0;	// number of coefficients in constraint different from 0

		for (int l = 0; l < nb_lectures; ++l) // sum(l in L)
		{
			//因为上面是双层循环添加的变量，只有z1_1,z2_1，z3_1是这部分约束（1）需要涉及的变量
			matind[nonzeros] = l * nb_rooms + r;	// variable a_lr is at position l*nb_rooms + r in the constraint matrix
			matval[nonzeros] = 1;					// a_lr has a coefficient of 1 in this constraint
			++nonzeros;		// increase variable nonzeros by 1, as we have a variable with nonzero coefficient in the constraint
		}

		// add the constraint to CPLEX
		status = CPXaddrows(env, pricing_problem, 0, 1, nonzeros, rhs, sense, matbeg, matind.get(), matval.get(), NULL, rowname);
		if (status != 0)
		{
			CPXgeterrorstring(env, status, error_text);
			throw runtime_error("Error in function build_pricing_problem(). \nCouldn't add constraint " + name + ". \nReason: " + string(error_text));
		}
	}

	// Constraint set 2: exactly one column selected for every timeslot
	for (int p = 0; p < nb_professors; ++p) // forall(p in P)
	{
		sense[0] = 'L';	// constraint is '<='
		rhs[0] = 1;		// right-hand side of constraint equals 1

		string name = "Professor_" + to_string(p + 1) + "_teaches_at_most_one_lecture";
		rowname[0] = const_cast<char*>(name.c_str());

		matbeg[0] = 0;	// always 0
		nonzeros = 0;	// number of coefficients in constraint different from 0

		for (int l = 0; l < nb_lectures; ++l) // sum(l in L)
		{
			if (professors_lecture.at(p * nb_lectures + l) == 1) // only lectures that are taught by this professor
			{
				for (int r = 0; r < nb_rooms; ++r) // sum(r in R)
				{
					matind[nonzeros] = l * nb_rooms + r;	// variable a_lr is at position l*nb_rooms + r in the constraint matrix
					matval[nonzeros] = 1;					// a_lr has a coefficient of 1 in this constraint
					++nonzeros;		// increase variable nonzeros by 1, as we have a variable with nonzero coefficient in the constraint
				}
			}
		}

		// add the constraint to CPLEX
		status = CPXaddrows(env, pricing_problem, 0, 1, nonzeros, rhs, sense, matbeg, matind.get(), matval.get(), NULL, rowname);
		if (status != 0)
		{
			CPXgeterrorstring(env, status, error_text);
			throw runtime_error("Error in function build_pricing_problem(). \nCouldn't add constraint " + name + ". \nReason: " + string(error_text));
		}
	}


	// Write the problem to an LP-file, so that we can check whether it is correct
	status = CPXwriteprob(env, pricing_problem, "pricing_problem.lp", NULL);
	if (status != 0)
	{
		CPXgeterrorstring(env, status, error_text);
		throw runtime_error("Error in function build_pricing_problem(). \nFailed to write the problem to a file. \nReason: " + string(error_text));
	}
}





/* ==================================================================
						SOLVE THE MASTER PROBLEM
   ================================================================== */

void solve_master_problem()
{
	int status = 0;
	int solstat;

	// Assign memory for solution
	// make_unique 同 unique_ptr 、auto_ptr 等一样，都是 smart pointer，可以取代new 并且无需 delete pointer，有助于代码管理
	solution_masterproblem = make_unique<double[]>(CPXgetnumcols(env, master_problem));


	// Optimize the problem
	cout << "\n\nCPLEX is solving the master problem ... ";
	status = CPXlpopt(env, master_problem);
	if (status != 0)
	{
		CPXgeterrorstring(env, status, error_text);
		throw runtime_error("Error in function solve_master_problem(). \nCPXlpopt failed. \nReason: " + string(error_text));
	}

	// Get the solution
	status = CPXsolution(env, master_problem, &solstat, &objective_value_masterproblem, solution_masterproblem.get(), dual_prices.get(), NULL, NULL);

	if (status != 0)
	{
		CPXgeterrorstring(env, status, error_text);
		throw runtime_error("Error in function solve_master_problem(). \nCPXsolution failed. \nReason: " + string(error_text));
	}


	// Check the solution status
	bool solution_exists;
	string solution_info;

	if (solstat == CPX_STAT_OPTIMAL) {
		solution_exists = true;
		solution_info = "Optimal solution found";
	}
	else if (solstat == CPXMIP_INFEASIBLE) {
		solution_exists = false;
		solution_info = "Problem is infeasible";
	}
	else if (solstat == CPX_STAT_INFEASIBLE) {
		solution_exists = false;
		solution_info = "Problem is infeasible";
	}
	else if (solstat == CPX_STAT_UNBOUNDED) {
		solution_exists = false;
		solution_info = "Problem is unbounded";
	}
	else if (solstat == CPX_STAT_INForUNBD) {
		solution_exists = false;
		solution_info = "Problem is infeasible or unbounded";
	}
	else {
		solution_exists = false;
		solution_info = "Other reason for failure";
	}

	if (!solution_exists)
		throw runtime_error("CPLEX failed solving the master problem. Reason: " + solution_info);


	cout << "\nCPLEX has finished: " << solution_info << "\n";
}





/* ==================================================================
					CHANGE COEFFICIENT PRICING PROBLEM
   ================================================================== */

void change_coefficients_pricing_problem(Problem &p)
{
	//修改路径成本后，调用最短路
//求解结果，转换成reduced cost、主问题目标函数系数、主问题中每个方案包含哪些点
	int nonzeros = 0;

	//根据结果修改路径成本（但不要破坏原来的）
	//每个点出发的边的新cost=原本该点出发的边的cost-dual price
	//起点不更新，就是客户点
	//遍历所有客户点，更新该客户点出发的边
	for (int i = 1; i <= p.m_customer_number; i++) {
		for (int j = 1; j <= p.m_customer_number+p.m_depot_number; j++) {
			if (i != j) {
				p.m_distance_new[i][j] = p.m_distance[i][j] - dual_prices[i-1];
				// dual_prices里50个约束对应50个对偶变量，对应50个客户，但是是从0开始计数
			}

		}
	}

	//下面就是求解新的最短路


	//// coefficients for a_lr
	//for (int l = 0; l < nb_lectures; ++l)
	//{
	//	for (int r = 0; r < nb_rooms; ++r)
	//	{

	//		double value = cost_lecture_timeslot.at(l * nb_timeslots + timeslot) - dual_prices[l];
	//		new_coefficients_pricingproblem[l * nb_rooms + r] = value;
	//		++nonzeros;
	//	}
	//}

}





/* ==================================================================
						SOLVE THE PRICING PROBLEM
   ================================================================== */

double solve_pricing_problem(Problem &p)
{
	//修改路径成本后，调用最短路
//求解结果，转换成reduced cost、主问题目标函数系数、主问题中每个方案包含哪些点
	Route r;
	double Reduced_cost_temp;
	Shortest_Path(p, r);
	Reduced_cost_temp = r.m_obj;


	cout << "print shortest route" << endl;
	for (int ii = 0; ii < r.size(); ii++) {
		cout << r[ii] << ", ";


	}
	cout << endl;
	//归集route，和column的编号一一对应，到时候用column的编号可以读取
	route_list.push_back(r);

	cout << "\solve_pricing_problem! " << endl;

	//cost_new_column是计算该路径的【真实成本】，对应放到目标函数里
	//循环route,看经历了哪些点。
	// calculate cost of new column and fill values of new column
	cost_new_column = 0;
	for (int i = 0; i < p.m_customer_number; ++i)
	{
		new_column_a.at(i) = 0;
	}
	//车库到第一个客户
	//cout << "here 3" << endl;
	cost_new_column += p.m_distance[r[0]][r[1]];
	//从第一个客户到最后一个客户
	for (int i = 1; i <= r.size()-2; ++i)
	{
		//cout << r[i] << endl;
		//客户1对应的0号约束！
		new_column_a.at(r[i]-1) = 1;
		cost_new_column += p.m_distance[r[i]][r[i + 1]];

	}
	cout << "cost_new_column=" << cost_new_column << endl;
	//cout << "here 4" << endl;
	//这是基于更改后成本的
	return Reduced_cost_temp;
}





/* ==================================================================
					  ADD COLUMN TO MASTER PROBLEM
   ================================================================== */
//【根据子问题的求解，改变主问题】
double add_column_to_master_problem(const Problem &p)
{
	int status = 0;
	double obj[1];						// Objective function coefficient of variables
	double lb[1];						// Lower bounds of variables
	double ub[1];						// Upper bounds of variables
	char *colname[1];					// Variable names
	int matbeg[1];						// Begin position of the constraint (always 0)
	int matind[2000];					// Position of each element in constraint matrix
	double matval[2000];				// Value of each element in constraint matrix
	int nonzeros = 0;					// To calculate number of nonzero coefficients in each constraint


	// Add the new column
	//添加的用z_1_1类似的表达
	string name = "z_" + to_string(nb_columns_added + 1);
	colname[0] = const_cast<char*>(name.c_str());

	matbeg[0] = 0;

	obj[0] = cost_new_column;

	lb[0] = 0;
	ub[0] = 1;

	// Constraint set 1: every lecture assigned to exactly one timeslot
	//matbeg (beginning of the matrix), matcnt (count of the matrix), matind (indices of the matrix), and matval (values of the matrix),

	//该列添加到前p.m_customer_number行
	//对于每行都判断（matind[nonzeros] = i;），是否包含（matval[nonzeros] = new_column_a.at(i);），如果不包含aij=0，zi也不会在里面
	for (int i = 0; i < p.m_customer_number; ++i)
	{
		//For each k, matind[k] indicates 【the row number】 of the corresponding coefficient, matval[k].
		//Duplicate entries in matind within a single column are not allowed
		//
		matind[nonzeros] = i;
		//这个是是否包含点i，如果包含则是1，否则为0.如果包含，系数matind[nonzeros]对应也是1
		matval[nonzeros] = new_column_a.at(i);
		++nonzeros;
	}

	// Constraint set 2: exactly one column per timeslot selected
	//matind[nonzeros] = nb_lectures + timeslot;
	//matval[nonzeros] = 1;
	//++nonzeros;
	cout << "here 1"<< endl;
	//这一列增加到 前nonzeros行，对应的x前aij是0还是1，以及系数是几
	status = CPXaddcols(env, master_problem, 1, nonzeros, obj, matbeg, matind, matval, lb, ub, colname);
	cout << "here 2" << endl;
	if (status != 0)
	{
		CPXgeterrorstring(env, status, error_text);
		throw runtime_error("Error in function add_column_to_master_problem(). \nCouldn't add variable " + name + ". \nReason: " + string(error_text));
	}

	// Write the problem to an LP-file, so that we can check whether it is correct
	status = CPXwriteprob(env, master_problem, "master_problem.lp", NULL);
	if (status != 0)
	{
		CPXgeterrorstring(env, status, error_text);
		throw runtime_error("Error in function add_column_to_master_problem(). \nFailed to write the problem to a file. \nReason: " + string(error_text));
	}
}





/* ==================================================================
							CLEAR CPLEX
   ================================================================== */

void clear_cplex()
{
	int status = 0;

	// Free the master problem
	status = CPXfreeprob(env, &master_problem);
	if (status != 0)
	{
		CPXgeterrorstring(env, status, error_text);
		throw runtime_error("Error in function clear_cplex(). \nCouldn't free the CPLEX master problem. \nReason: " + string(error_text));
	}

	// Free the pricing problem
	status = CPXfreeprob(env, &pricing_problem);
	if (status != 0)
	{
		CPXgeterrorstring(env, status, error_text);
		throw runtime_error("Error in function clear_cplex(). \nCouldn't free the CPLEX pricing problem. \nReason: " + string(error_text));
	}

	// Close the cplex environment
	status = CPXcloseCPLEX(&env);
	if (status != 0)
	{
		CPXgeterrorstring(env, status, error_text);
		throw runtime_error("Error in function clear_cplex(). \nCouldn't close CPLEX environment. \nReason: " + string(error_text));
	}
}





/* ==================================================================
							WRITE OUTPUT
   ================================================================== */

void write_output(const Problem &p)
{
	// To file
	{
		ofstream file;
		file.open("output_column_generation.txt");
		if (!file.is_open())
			throw runtime_error("Couldn't open output file!");

		file << "Computation time (seconds)\t" << computation_time;
		file << "\nNumber of columns added\t" << nb_columns_added;
		file << "\nObjective value optimal solution\t" << objective_value_masterproblem;
	}

	// To screen
	{
		//输出初始解
		for (int i = 0; i < CPXgetnumcols(env, master_problem); i++) {
			//cout << solution_masterproblem[i] << endl;
			//因为初始解占了前p.GetCustomerNumber()（50）个
			if (solution_masterproblem[i+p.GetCustomerNumber()] == 1) {
				for (int j = 0; j < route_list[i].size(); j++) {
					cout << route_list[i][j] << ", ";
				}
				cout << endl;
			}
		}

		cout << "\n\n\nColumn generation has finished";
		cout << "\nComputation time (seconds): " << computation_time;
		cout << "\nNumber of columns added: " << nb_columns_added;
		cout << "\nObjective value optimal solution: " << objective_value_masterproblem;
	}
}





/* ==================================================================
						COLUMN GENERATION LOOP
   ================================================================== */

void column_generation(Problem &p)
{
	// start a timer
	auto start_time = chrono::system_clock::now();

	initialize_cplex();
	build_master_problem(p);
	//build_pricing_problem();

	// column generation loop
	nb_columns_added = 0;
	int iteration = 0;
	while (true)
	{
		++iteration;
		cout << "\n\n\n\n\nColumn generation iteration: " << iteration << "\n";

		solve_master_problem();

		// for every timeslot, we need to solve a pricing problem
		bool new_columns_found = false; // remember whether we found new columns or not


		//改变定价子问题（主要是更新price）
		change_coefficients_pricing_problem(p);
		//求解子问题得到reduced cost，如果他小于0，则增加该方案到主问题
		double reduced_cost = solve_pricing_problem(p);

		// if the reduced cost of the problem is < 0 (+ rounding errors)
		if (reduced_cost < -0.0001)
		{
			cout << "New column found with reduced cost: " << reduced_cost;

			// if we have found a new column with negative reduced cost, we add it to the master problem
			add_column_to_master_problem(p);
			
			// we have found a new column
			new_columns_found = true;
			++nb_columns_added;
		}

		// if we didn't find any new columns, we can stop
		if (!new_columns_found)
			break; // exit the column generation loop ("while(true)")
	}

	// Get the elapsed time
	chrono::duration<double, ratio<1, 1>> elapsed_time = chrono::system_clock::now() - start_time;
	computation_time = elapsed_time.count();

	// write output to screen and file
	write_output(p);

	// clear cplex
	clear_cplex();


}





/* ==================================================================
								 MAIN
   ================================================================== */

   // Program execution always starts at "main"
int main()
{

	
	
	try
	{
		Problem p;
		p.LoadVRPTW("vrpnc1");

		//读入数据后直接进行列生成循环
		// Then try to solve the problem with column generation
		column_generation(p);

	}
	catch (const exception& ex)
	{
		// An error occurred: write to screen what went wrong
		cout << "\n\n" << ex.what();
	}

	cout << "\n\n\n\n\nPress enter to exit the program ... ";
	getchar();
	getchar();
	return 0; // program ends here
}



