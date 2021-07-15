

#include "Customer.h"

Customer::Customer() :
		m_id(0), m_weight(0),
		m_place_x(0), m_place_y(0),
		m_early_time(0), m_late_time(100000),
		m_service_time(0)
{
}

Customer::~Customer()
{
}
