#pragma once
#pragma warning(disable:4996)
#include "Solution.h"
#include "Conf.h"
#include "Utility.h"
#include "Feasibility.h"
#include <cmath>
#include <cstdlib>
#include <iomanip>
#include <algorithm>

Route::Route() :
		m_obj(0), depot_id(0)//, m_obj(0)
{
	//this->push_back(0);//route��ʼ���������������
	//this->push_back(0);
	//���ڳ�ʼ��������0�����ǡ��ڵ������ĳ����������Ӧ���⡿��
}

Route::~Route()
{
}
bool Solution::Empty()
{
	//return this->size() < 3;
	return this->m_obj == 0;
}
bool Route::Empty()
{
	return int(this->size()) < 3;
}

bool Route::Empty()const
{
	return this->size() < 3;
}

void Route::Debug(std::ostream &output) const
{
	output << "Route Information: cost = " << m_obj << ", customer number = " << this->size() - 2 << "\n\t order : ";
	for(unsigned j = 0; j < this->size(); ++j)
	{
		output << " " << this->at(j);//Ϊ��debug��ӡ��·���ĳɱ����ͻ�����������Ŀͻ�˳��
	}
	output << std::endl;
}

Solution::Solution() :
		m_obj(0)// m_obj(0), m_penalty_obj(0),
{
}

Solution::Solution(double cost) :
		m_obj(cost)//m_obj(cost), m_penalty_obj(cost),
{
}

Solution::~Solution()
{
}

void Solution::Output(std::ostream &output)
{
	int used_vehicle = 0;
	for(unsigned i = 0; i < this->size(); ++i)
	{
		Route &route = this->at(i);
		if(route.Empty()) //dummy route only contains two depots
		{
			continue;
		}

		++used_vehicle;
		output << "Route " << used_vehicle << " : obj = " << this->at(i).m_obj << ", customer number = " << this->at(i).size() - 2 << "\t";
		for(unsigned j = 0; j < route.size(); ++j)
		{
			output << " " << route[j];
		}
		output << std::endl;
	}
	output << std::endl << "the solution obj = " << m_obj <<  ", time = " << Conf::run_time << std::endl << std::endl;
	

}

void Solution::Output(std::ostream &output)const
{
	int used_vehicle = 0;
	for(unsigned i = 0; i < this->size(); ++i)
	{

		if(this->at(i).Empty()) //dummy route only contains two depots
		{
			//continue;
		}

		++used_vehicle;
		output << "Route " << used_vehicle << " : obj = " << this->at(i).m_obj << ", customer number = " << this->at(i).size() - 2 << "\t";
		for(unsigned j = 0; j < this->at(i).size(); ++j)
		{
			output << " " << this->at(i).at(j);
		}
		output << std::endl;
	}
	output << std::endl << "the solution obj = " << m_obj << ", Sum" << ", time = " << Conf::run_time << std::endl << std::endl;

}

void Solution::Output(const std::string &file_name, const Problem &p)
{
	char suffix[32];
	sprintf(suffix, "-%d.txt", 0);
	std::string outfile = "result/" + file_name + suffix;
	std::ofstream fout(outfile.c_str());
	Output(fout);
	fout.close();
}

void Solution::OutputPerformance(const std::string &file_name, const std::string &method, const Problem &p)
{
	int nVehicle = 0;
    for(unsigned i = 0; i < this->size(); ++i)
    {
    	if (!this->at(i).Empty())
    	{
    		++nVehicle;
    	}
    }
	char fname[32];
	sprintf(fname, "result.txt");
	std::ofstream fout(fname, std::ofstream::app);
	fout << p.m_instance_name << "\t" << method <<"this is exits"
		 << std::endl;
}

//solution.verify�ص���
void Solution::VerifyRoutes(const Problem& p, double &solution_cost)
{
	std::cout << "\t &&& begin to check route... : " << m_obj << std::endl;

	//double solution_obj = 0;
	//û�пͻ�0��50���ͻ���1��50��Ŷ�Ӧused[1]��used[50]��������Ҫp.m_customer_number + 1��λ�ã���used[0]��һ����
	std::vector< bool > used = std::vector< bool >(p.m_customer_number + 1, false);
	solution_cost=0;
	for(unsigned i = 0; i < this->size(); ++i)
	{
		double total_weight = 0;
		//ÿ��·���ڣ��ӵ�һ���ͻ������һ���ͻ�����һ�����ǳ��⣬���һ��Ҳ�ǣ�
		solution_cost += p.m_distance[this->m_id][this->at(i).at(1)];
		for(unsigned j = 1; j  <= this->at(i).size()-2; ++j)
		{
			solution_cost+= p.m_distance[this->at(i).at(j)][this->at(i).at(j+1)];
			if(this->at(i).at(j) > p.m_customer_number || this->at(i).at(j)<=0)//==0���������0��50���ͻ���MDVRP��51�ǳ���
			{
				cout << "Route Error : depot appears among the route" << i << endl;
				exit(1);
				//Utility::OutPutErrorMsg("Route Error: depot appears among the route(%d)!", i);
			}

			if(used[this->at(i).at(j)])
			{
				cout << "Route Error: customer" << i << "appears twice!" << endl;
				exit(1);
				//Utility::OutPutErrorMsg("Route Error: customer(%d) appears twice!", this->at(i).at(j));
			}
			//�ؼ�
			used[this->at(i).at(j)] = true;
			total_weight += p.GetCustomer(this->at(i).at(j)).m_weight;
		}

		if(total_weight > p.GetVehicle().m_capacity)
		{
			std::cout << "Route Capacity Error: in route"<<i<<" current:"<<total_weight<<" -- capacity:"<<p.GetVehicle().m_capacity << endl;
			exit(1);
			//Utility::OutPutErrorMsg("Route Capacity Error: in route(%d) current:%f -- capacity:%d", i, total_weight, p.GetVehicle().m_capacity);
		}

	}

	for(int i = 1; i <= p.m_customer_number; ++i)
	{
		//����ĸ��ͻ�û����ǵ���˵��©����
		if(!used[i])
		{
			cout << "Route Error: customer"<<i<<" does not appear!" << endl;
			exit(1);
			//Utility::OutPutErrorMsg("Route Error: customer(%d) does not appear!", i);
		}
	}

	if( int(this->size()) > p.GetVehicleNumber() )
	{
		cout << "Route Number Error: obtained routes is "<<this->size()<<" while actual routes is "<< p.GetVehicleNumber() << endl;
		//Utility::OutPutErrorMsg("Route Number Error: obtained routes is (%d) while actual routes is (%d)!", this->size(), p.GetVehicleNumber());
		exit(1); //debug
	}
	cout << "The cost of solution  is" << solution_cost << endl;
	//�ٴμ���ɱ�
	//ʱ�䴰���������ж�
}


