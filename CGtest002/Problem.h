#pragma once


#include "Vehicle.h"
#include "Customer.h"
#include "Depot.h"
#include <vector>
#include <string>

class Problem
{
public:
	Problem();
	~Problem();


	void LoadVRPTW(const std::string &file_name);
	double GetDistance(const int c1, const int c2)const;
	double GetAvgDistance(const int c)const;
	int GetVehicleNumber()const;
	const Vehicle &GetVehicle()const;

	int GetCustomerNumber()const;
	const Customer &GetCustomer(const int cid)const;

	int GetDepotNumber()const;
	 Depot &GetDepot( int did);



private:
	void CalculateDistance();






public:

	std::string m_instance_name;
	int m_class;   //instance information


	int m_customer_number;
	int m_depot_number;
	int m_item_number;

	Vehicle m_vehicle;

	std::vector< Customer > m_customer;
	std::vector< Depot > m_depot;
	int m_freq_count;
	std::vector< std::vector<double> > m_freq;
	std::vector< std::vector<double> > m_distance;
	//用作子问题更新
	std::vector< std::vector<double> > m_distance_new;
	std::vector<double> m_avgCost;

};


