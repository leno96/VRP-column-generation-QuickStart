

#include "Depot.h"

Depot::Depot() :
		m_id(0),v_id(0),
		m_place_x(0), m_place_y(0)
{
}

Depot::Depot(int id,  vector<Customer> customers) //, vector<Vehicle> vehicles
{
	this->m_id = id;
	//this->m_place_x = x;
	//this->m_place_y = y;
	this->customers = customers;
	//this->m_vehicle = vehicles;
}

Depot::~Depot()
{
}
