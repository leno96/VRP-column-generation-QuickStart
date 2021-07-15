#pragma once

using namespace std;

#include <vector>
#include "Vehicle.h"
#include "Customer.h"


class Depot
{
public:
	Depot();
	Depot(int id, vector<Customer> customers);//, vector<Vehicle> vehicles
	~Depot();


	int m_id;

	double m_place_x;
	double m_place_y;    //the geographical position of customer

	int v_id;
	vector<Customer> customers;
	//vector<Vehicle> m_vehicle;


	//double m_early_time;	//time window
	//double m_late_time;
	//double m_service_time;

};


