

#pragma once

#include <vector>



class Customer
{
public:
	Customer();
	~Customer();

	int m_id;
	double m_weight;

	double m_place_x;
	double m_place_y;    //the geographical position of customer

	double m_early_time;	//time window
	double m_late_time;
	double m_service_time;

};
