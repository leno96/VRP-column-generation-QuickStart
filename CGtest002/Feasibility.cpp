
#pragma once
#include "Feasibility.h"
#include "Conf.h"
#include <iostream>
#include <algorithm>
#include <functional>
#include <cmath>




bool IsFeasible_Weight(const Route &r, const Problem &p)
{
	double weight = 0;
	for (unsigned i = 1; i < r.size() - 1; ++i)
	{
		weight += p.GetCustomer(r[i]).m_weight;
	}
	//cout << "weight=" << weight << endl;
	return weight <= p.GetVehicle().m_capacity;
}

double IsFeasible_Weight(const Route &r, const Problem &p, bool feasible)
{
	double weight = 0;
	for (unsigned i = 1; i < r.size() - 1; ++i)
	{
		weight += p.GetCustomer(r[i]).m_weight;
	}
	//cout << "weight=" << weight << endl;
	return weight - p.GetVehicle().m_capacity;
}

void print_shortest_route(const Problem &p)
{
	Route r_temp;
	//cout << "before Shortest_Path" << endl;
	Shortest_Path( p, r_temp);
	cout << "print select route" << endl;
	for (int ii = 0; ii < r_temp.size(); ii++) {
		cout << r_temp[ii] << ", ";

	}
	cout << endl;
}

bool IsFeasible(Route &r, const Problem &p)
{
	//r.Debug(std::cout); //debug


	//cout<<"is feasible?:"<< IsFeasible_Weight(r, p)<<endl;
	return IsFeasible_Weight(r, p);

	

}

//����Ͳ���route�ˣ���������ͼ
//Ҫ����route���������route���ṩ1.·����ʵ�ɱ������ݰ�����Щ�߼��㣩��2.·����Ӧ��reduced cost��ֱ�ӷ��أ���3.������Щ�ͻ���
void Shortest_Path(const Problem &p, Route &r_select)
{
	cout << "short path" << endl;
	r_select.clear();
	
	//�����value��Ϊ�˼�¼�����˼���
	//һ�㶼�Ǵӵ�һ����ʼ����
	//trie.is_included(r, index, count_access)�������-1����ӵ�index��û���ҵ�
	
	Route r_rec;
	double route_obj;


	//Ĭ�Ͽͻ���+���������㣻��ע���Ǵ�0��ʼ������,����50���ͻ��㣬1�����⣬��0-50��50�ǳ���
	//�Ĺ�֮��0��Ϊ���⳵��㣬1-50�ͻ���51Ҳ�����⳵���
	AdjMatrixDirNetwork network( p.m_customer_number+ p.m_depot_number+1, p.m_customer_number + p.m_depot_number+1);
	//�з�����Ϊ��i�����ı��Ǽ�ȥ��i��dual cost
	//����ij����ͼ
	//�ĳ�0-51��0��51���ǳ���
	for (int i = 0; i <= p.m_customer_number + p.m_depot_number; i++) {
		for (int j = 0; j <= p.m_customer_number + p.m_depot_number; j++) {
			//name�ȱ�Ŵ�һ������0�ŵ��Ӧ�ͻ�1��50�ŵ��Ӧ����51;m_distance_new�ǰ����ֱ�ĺ�
			//���·�����������ƣ�
			if (i == 0) {
				if (j <= p.m_customer_number) {
					network.InsertArc(0, j, 0, j, p.m_distance_new[p.m_customer_number + p.m_depot_number][j], p.m_customer[j].m_weight);
				}
				else {
					network.InsertArc(0, j, 0, j, p.m_distance_new[p.m_customer_number + p.m_depot_number][j], 0);
				}
				if (j == 0) {
					network.InsertArc(0, 0, 0, 0, INT_MAX, 0);
				}
			}
			else {
				if (j <= p.m_customer_number) {
					network.InsertArc(i, j, i, j, p.m_distance_new[i][j], p.m_customer[j].m_weight);
				}
				else {
					network.InsertArc(i, j, i, j, p.m_distance_new[i][j], 0);
				}
				if (j == 0) {
					//�κε㵽��㳵�ⶼ�������ľ��루���ܷ�����㳵�⣩
					network.InsertArc(i, 0, i, 0, INT_MAX, 0);
				}
			}

			//if (i < p.m_customer_number) {
			//	network.InsertArc(i + 1, j + 1, i, j, p.m_distance_new[i + 1][j + 1], p.m_customer[i+1].m_weight);
			//}
			//else {
			//	network.InsertArc(i + 1, j + 1, i, j, p.m_distance_new[i + 1][j + 1], 0);
			//}
			


		}
	}

	//network.Display();
	//�ؼ�����ô����weight�����·

	//double profit, cost;
	//for (int i = 0; i < r.size(); i++) {
	//	//ֻ������·��������
	//	for (int j = i + 1; j < r.size(); j++) {
	//		if (i == 0) {
	//			if (j == r.size() - 1) {
	//				//������ӳ���ֱ�ӵ����⣬��˲���������
	//				//���������ߣ�����������select����һ���ܴ�ĸ�ֵ

	//				//����ӳ��⵽���⣬�ɱ���������������0

	//				//if (r[i] != 101 || r[j] != 101) {
	//				//	cout<<"r[j]=" << r[j] << endl;
	//				//	exit(0);
	//				//}
	//				network.InsertArc(r[i], r[j], i, j, 0, 0);
	//			}
	//			if (j != r.size() - 1) {
	//				cost = p.GetDistance(r[i], r[j]);
	//				profit = (p.m_customer[r[j]].m_profit) / 2;
	//				network.InsertArc(r[i], r[j], i, j, cost - profit, (p.m_customer[r[j]].m_weight) / 2);

	//			}
	//		}
	//		if (i != 0) {
	//			if (j == r.size() - 1) {
	//				cost = p.GetDistance(r[i], r[j]);
	//				//cout << "r[j]=" << r[j] << endl;
	//				profit = (p.m_customer[r[i]].m_profit) / 2;
	//				//j�����һλ���ֿ��key�͵�һλ��key��һ���ģ���id��һ����id�����һλ�����һλ��ָ���κΡ�
	//				network.InsertArc(r[i], r[j], i, j, cost - profit, (p.m_customer[r[i]].m_weight) / 2);

	//			}
	//			if (j != r.size() - 1) {
	//				cost = p.GetDistance(r[i], r[j]);
	//				profit = (p.m_customer[r[i]].m_profit + p.m_customer[r[j]].m_profit) / 2;
	//				network.InsertArc(r[i], r[j], i, j, cost - profit, (p.m_customer[r[j]].m_weight + p.m_customer[r[j]].m_weight) / 2);

	//			}
	//		}
	//	}
	//}
	vector<int> parent;
	//int num = r.size();
	double obj_rec = -1;

	//network.Display();
	Route route_rec_id;

	//��0�㿪ʼ��Ѱ
	obj_rec = ShortestPathBellmanFord(network, route_rec_id, 0);


	//network.Display();



	Route r_temp;

	//cout << "route_rec_id_2=" << endl;
	//�����⳵��0����ʵ�ʳ�����51
	r_temp.push_back(p.m_customer_number + p.m_depot_number);
	for (int ii = route_rec_id.size() - 2; ii >= 0; ii--) {
		r_temp.push_back(route_rec_id[ii]);
		//cout << route_rec_id[ii] << ",";
	}
	//�����һ������
	r_temp.push_back(p.m_customer_number + p.m_depot_number);

	//for (int ii = 0; ii <= r_temp.size() - 1; ii++) {
	//	cout << r_temp[ii] << ",";

	//}
	//cout << endl;

	r_temp.m_obj = obj_rec;

	r_select = r_temp;

	//cout << "print select route" << endl;
	//for (int ii = 0; ii < r_select.size(); ii++) {
	//	cout << r_select[ii] << ", ";
	//

	//}
	//cout << endl;
	cout << "r_select.obj=" << r_select.m_obj << endl;





	//if (r_temp.size() > 2) {
	//	////|| IsFeasible_Weight(r_temp, p) == false
	//	////����&ɾ����obj������С���ۺϿ���dist��profit
	//	//while (IsFeasible_Weight(r_temp, p) == false) {
	//	//	double min_obj = 20000;// -INT_MAX;
	//	//	int best_delete = -1;
	//	//	//////Ϊʲô�����1��ʼ������Ϊ��һ�����ǳ��⣬�ڶ���������ǿͻ�������ÿ������ɾ����λ��
	//	//	//for (unsigned i = 1; i < r_temp.size() - 1; i++) {
	//	//	//	Route r_temp_2(r_temp);//����·��r��һ��temp·������r copy����������Ӱ��r��route����solution�д�����
	//	//	//	//i=1ʱ����ʾ���ڵ�1������棻�������������һ��3���㣬�����ڵ�2�������				
	//	//	//	double obj_delta = p.m_customer[r_temp_2[i]].m_profit + (p.GetDistance(r_temp_2[i - 1], r_temp_2[i + 1]) - p.GetDistance(r_temp_2[i - 1], r_temp_2[i]) - p.GetDistance(r_temp_2[i], r_temp_2[i + 1]));
	//	//	//	//r_temp_2.erase(r_temp_2.begin() + i);
	//	//	//	//����obj������0

	//	//	////double obj_delta = r_temp.m_obj- r_temp_2.m_obj;//�ԱȲ���r��iλ�ú�Ŀ�꺯����������·����Ŀ�꺯�������Ѹÿͻ�ɾ��·���󣩼��ٵ�obj
	//	//	//	if (obj_delta < min_obj)
	//	//	//	{
	//	//	//		min_obj = obj_delta;
	//	//	//		best_delete = i;	//�ҵ�һ��Ӱ����С��ɾ��·��λ��
	//	//	//	}


	//	//	//}
	//	//	//if (best_delete == -1) {
	//	//	//	cout << "best_delete == -1" << endl;
	//	//	//	exit(0);
	//	//	//}
	//	//	best_delete = 1;
	//	//	Route r_temp_2(r_temp);
	//	//	min_obj=p.m_customer[r_temp_2[1]].m_profit + (p.GetDistance(r_temp_2[1 - 1], r_temp_2[1 + 1]) - p.GetDistance(r_temp_2[1 - 1], r_temp_2[1]) - p.GetDistance(r_temp_2[1], r_temp_2[1 + 1]));
	//	//	r_temp.erase(r_temp.begin() + best_delete);
	//	//	r_temp.m_obj = r_temp.m_obj - min_obj;

	//	//����һ�����صĳͷ�
	//	r_temp.m_obj = r_temp.m_obj - 20*(std::max)(IsFeasible_Weight(r_temp, p, true), 0.0);


	//	
	//	r_select = r_temp;
	//	//r.m_cost = 0;
	//	//for (unsigned jj = 0; jj <= r.size()-2; jj++)
	//	//{
	//	//	r.m_cost += p.m_distance[r[jj]][r[jj + 1]];
	//	//
	//	//}


	//}
	//else {
	//	r_temp.m_obj = 0;
	//	//r.m_cost = 0;
	//	r_select.m_obj = 0;
	//	r_select.push_back(r[0]);
	//	r_select.push_back(r[0]);
	//}



	//for (int ii = 0; ii < route_rec_id.size(); ii++) {
	//	r_temp.push_back(r[route_rec_id[ii]]);
	//}


	//cout << "print select route" << endl;
	//for (int ii = 0; ii < r_select.size(); ii++) {
	//	cout << r_select[ii] << ", ";
	//	

	//}
	//cout << endl;
	//cout << "r_select.obj=" << r_select.m_obj << endl;

	//cout << "finish out" << endl;
	//(const Route& r, unsigned index, const Route &r_rec, double route_obj)






}








bool IsFeasible_Time(const Route &r, const Problem &p)
{
	double tstart = p.GetCustomer(0).m_early_time;
	for (unsigned i = 1; i < r.size(); ++i)
	{
		tstart += p.GetCustomer(r[i - 1]).m_service_time;
		tstart += p.GetDistance(r[i - 1], r[i]);
		Customer cust = p.GetCustomer(r[i]);
		if (tstart > cust.m_late_time + 1e-3)
		{
			return false;
		}
		tstart = std::max(tstart, cust.m_early_time);
	}
	return true;
}

//-------------------------- Evaluate Obj ----------------------------
double CalculateRouteCost(Route &r, const Problem &p)
{
	r.m_obj = 0;

	if (r.Empty()) //only contains the two depots
	{
		return 0;
	}

	//1�ǵ�һ���ͻ���r.size()-2�����һ���ͻ���r.size()-1�ǳ��⡣
	//���r.size==4,0-7-3-0,r[1]�ǵ�һ���ͻ���r[2]�����һ���ͻ���
	//r[0]�������51���Ѿ���ǰ��������
	r.m_obj += p.GetDistance(r[0], r[1]);
	for (unsigned k = 2; k <= r.size() - 2; ++k)
	{
		r.m_obj += p.GetDistance(r[k - 1], r[k]);
	}
	//r.m_obj += p.GetDistance(r[r.size()-2],r[r.size() - 1] );
	r.m_obj += p.GetDistance(r[r.size() - 2], r[0]);
	//cout << "depot" << r[0] << "==" << r[r.size() - 1] << "; No.1 cus" << r[1] << " ,cost="<<r.m_obj<<endl;
	return r.m_obj;
}







//----------------------------Solution-----------------------
void CalculateSolutionCost(Solution &sol, const Problem &p)
{
	sol.m_obj = 0;
	for (unsigned i = 0; i < sol.size(); ++i)
	{
		Route &route = sol[i];
		sol.m_obj += CalculateRouteCost(route, p);
	}
}
void CalculateSolutionObj(Solution &sol, const Problem &p)
{
	sol.m_obj = 0;
	for (unsigned i = 0; i < sol.size(); ++i)
	{
		Route &route = sol[i];
		//for (int ii = 0; ii <= route.size() - 1; ii++) {
		//	cout << route[ii] << ",";

		//}
		//cout << endl;
		sol.m_obj += route.m_obj;
	}
}