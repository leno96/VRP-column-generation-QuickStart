//小数位数需要注意下
//m_distance[i][m_customer_number+j]第二个维度注意。
#include "Problem.h"
#include <fstream>
#include <iostream>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <algorithm>
using namespace std;

Problem::Problem() :
		m_instance_name(""), m_class(-1),
		 m_customer_number(0),m_item_number(0),m_freq_count(0)
{
}

Problem::~Problem()
{
}

void Problem::LoadVRPTW(const std::string &file_name)
{
	m_instance_name = file_name;


	double buf;
	m_depot_number = 1;
	m_instance_name = file_name;
	std::string inputfile = "data/CMT/" + file_name + ".txt";


	std::ifstream fin(inputfile.c_str());
	fin >> m_customer_number;
	fin >> m_vehicle.m_capacity >> buf >>  buf;

	m_customer.resize(1 + m_customer_number);
	m_depot.resize(1 + m_depot_number);
	fin >>  m_depot[1].m_place_x >> m_depot[1].m_place_y ;
	for (int i = 1; i <= m_customer_number; ++i)
	{
		m_customer[i].m_id = i;
		fin >> m_customer[i].m_place_x >> m_customer[i].m_place_y >> m_customer[i].m_weight;
		//fin >> m_customer[i].m_early_time >> m_customer[i].m_late_time >> m_customer[i].m_service_time;

	}

	


	//读入的最后计算距离矩阵
	m_distance.clear();
	if (m_distance.empty())
	{
		std::cout << "calculate distance in CVRP instances." << std::endl;
		CalculateDistance();
	
	}
    //cout << "depot 1 -- cus 25: " << m_distance[1 + m_customer_number][25] << "|| " << m_distance[25][1 + m_customer_number] << endl;
	cout << "m_customer[1].m_place_x=" << m_customer[1].m_place_x << endl;
	cout << "m_customer[1].m_place_y=" << m_customer[1].m_place_y << endl;
	cout << "m_customer[4].m_place_x=" << m_customer[4].m_place_x << endl;
	cout << "m_customer[4].m_place_y=" << m_customer[4].m_place_y << endl;
	if (m_customer_number >= 50) {
		cout << "m_customer[50].m_place_x=" << m_customer[50].m_place_x << endl;
		cout << "m_customer[50].m_place_y=" << m_customer[50].m_place_y << endl;
	}
	//这里freq用不到
	m_freq.clear();
	m_freq.resize(m_customer_number + 1, std::vector< double >(m_depot_number + 1, 0));

	fin.close();




}



void Problem::CalculateDistance()
{
	//二维数组，首先定义大小.如果50个客户，3个车库，总共53个点，1-53，需要54个位置
	m_distance.resize(m_customer_number + m_depot_number+1, std::vector< double >(m_customer_number + m_depot_number+1, 0));
	m_distance_new.resize(m_customer_number + m_depot_number + 1, std::vector< double >(m_customer_number + m_depot_number + 1, 0));
	//m[1]到m[52]
	for(int i = 1; i <= m_customer_number + m_depot_number-1; ++i)
	{
		////m[2]到m[53]
		for(int j = i + 1; j <= m_customer_number + m_depot_number; ++j)
		{
			double distance;

			if (j <= m_customer_number && i <= m_customer_number) {
				double d_x = m_customer[i].m_place_x - m_customer[j].m_place_x;
				double d_y = m_customer[i].m_place_y - m_customer[j].m_place_y;
				distance = std::sqrt(d_x * d_x + d_y * d_y);
			}
			if (j > m_customer_number&&i <= m_customer_number) {
				double d_x = m_customer[i].m_place_x - m_depot[j- m_customer_number].m_place_x;
				double d_y = m_customer[i].m_place_y - m_depot[j - m_customer_number].m_place_y;
				distance = std::sqrt(d_x * d_x + d_y * d_y);
			}
			if (j <= m_customer_number&&i > m_customer_number) {
				cout << "there is an error!" << endl;
			}
			if (j > m_customer_number&&i > m_customer_number) {
				double d_x = m_depot[i - m_customer_number].m_place_x - m_depot[j - m_customer_number].m_place_x;
				double d_y = m_depot[i - m_customer_number].m_place_y - m_depot[j - m_customer_number].m_place_y;
				distance = std::sqrt(d_x * d_x + d_y * d_y);
			}
			
            //distance = (int)(distance * 10) / 10.0;		//round downward at the first decimal place
			m_distance[i][j] = m_distance[j][i] = distance;
			
		}
	}
	for (int i = 1; i <= m_customer_number + m_depot_number; ++i){
		//自身到自身的距离肯定是正无穷
		m_distance[i][i] = INT_MAX;
	}
	m_distance_new = m_distance;
}






double Problem::GetDistance(const int c1, const int c2)const
{
	return m_distance[c1][c2];
}

double Problem::GetAvgDistance(const int c)const
{
	return m_avgCost[c];
}



int Problem::GetVehicleNumber()const
{
	return m_vehicle.m_number;
}
const Vehicle &Problem::GetVehicle()const
{
	return m_vehicle;
}

int Problem::GetDepotNumber()const
{
	return m_depot_number;
}
 Depot &Problem::GetDepot (int did)
{
	return m_depot[did];
}


int Problem::GetCustomerNumber()const
{
	return m_customer_number;
}
const Customer &Problem::GetCustomer(const int cid)const
{
	return m_customer[cid];
}




