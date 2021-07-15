#pragma once



#include "Problem.h"
#include <vector>
#include <fstream>
#include <iostream>

//start and end at depot(0)
class Route : public std::vector< int >//Each element in the int vector is an object of this class, including these functions 
{
public:
	Route();
	~Route();

	bool Empty();//判断是否为空（路径的size<3，则只包含两个车库，为空）
	bool Empty() const;

	void Debug(std::ostream &output)const;
	int depot_id;

	double m_obj;
	double m_cost;
};

class Solution : public std::vector< Route >//route向量里每个元素都是这类的对象，每个元素是一个路径，路径有他对应的solution
{
public:
	Solution();
	Solution(double cost);
	~Solution();
	bool Empty();

	void Output(std::ostream &output);
	void Output(std::ostream &output)const;
	void Output(const std::string &file_name, const Problem &p);

	void OutputPerformance(const std::string &file_name, const std::string &method, const Problem &p);

	void VerifyRoutes(const Problem& p, double &solution_cost);

	double m_obj;
	int m_id;
	//double m_obj;


};

