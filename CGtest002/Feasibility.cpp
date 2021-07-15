
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

//这里就不是route了，而是整个图
//要返回route，根据这个route，提供1.路径真实成本（根据包含哪些边计算），2.路径对应的reduced cost（直接返回），3.包含哪些客户点
void Shortest_Path(const Problem &p, Route &r_select)
{
	cout << "short path" << endl;
	r_select.clear();
	
	//这里的value是为了记录访问了几次
	//一般都是从第一个开始搜索
	//trie.is_included(r, index, count_access)如果返回-1，则从第index个没有找到
	
	Route r_rec;
	double route_obj;


	//默认客户数+车库数个点；【注意是从0开始计数】,比如50个客户点，1个车库，是0-50，50是车库
	//改过之后0作为虚拟车库点，1-50客户，51也是虚拟车库点
	AdjMatrixDirNetwork network( p.m_customer_number+ p.m_depot_number+1, p.m_customer_number + p.m_depot_number+1);
	//有方向，因为从i出发的边是减去的i的dual cost
	//遍历ij构建图
	//改成0-51，0和51都是车库
	for (int i = 0; i <= p.m_customer_number + p.m_depot_number; i++) {
		for (int j = 0; j <= p.m_customer_number + p.m_depot_number; j++) {
			//name比编号大一，比如0号点对应客户1，50号点对应车库51;m_distance_new是按名字编的号
			//最短路有运载量限制！
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
					//任何点到起点车库都是无穷大的距离（不能返回起点车库）
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
	//关键是怎么考虑weight的最短路

	//double profit, cost;
	//for (int i = 0; i < r.size(); i++) {
	//	//只能沿着路径向后遍历
	//	for (int j = i + 1; j < r.size(); j++) {
	//		if (i == 0) {
	//			if (j == r.size() - 1) {
	//				//不允许从车库直接到车库，因此不构建连线
	//				//不构筑连线，导致了下面select返回一个很大的负值

	//				//如果从车库到车库，成本、运量、利润都是0

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
	//				//j是最后一位，仓库的key和第一位的key是一样的，但id不一样，id是最后一位。最后一位不指向任何。
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

	//从0点开始搜寻
	obj_rec = ShortestPathBellmanFord(network, route_rec_id, 0);


	//network.Display();



	Route r_temp;

	//cout << "route_rec_id_2=" << endl;
	//把虚拟车库0换成实际车库编号51
	r_temp.push_back(p.m_customer_number + p.m_depot_number);
	for (int ii = route_rec_id.size() - 2; ii >= 0; ii--) {
		r_temp.push_back(route_rec_id[ii]);
		//cout << route_rec_id[ii] << ",";
	}
	//这里就一个车库
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
	//	////遍历&删除后obj减少最小；综合考虑dist和profit
	//	//while (IsFeasible_Weight(r_temp, p) == false) {
	//	//	double min_obj = 20000;// -INT_MAX;
	//	//	int best_delete = -1;
	//	//	//////为什么这里从1开始？？因为第一个点是车库，第二个点才能是客户。遍历每个可以删除的位置
	//	//	//for (unsigned i = 1; i < r_temp.size() - 1; i++) {
	//	//	//	Route r_temp_2(r_temp);//根据路径r造一个temp路径，把r copy出来，以免影响r【route类在solution中创建】
	//	//	//	//i=1时，表示插在第1个点后面；如果连车库在内一共3个点，最多插在第2个点后面				
	//	//	//	double obj_delta = p.m_customer[r_temp_2[i]].m_profit + (p.GetDistance(r_temp_2[i - 1], r_temp_2[i + 1]) - p.GetDistance(r_temp_2[i - 1], r_temp_2[i]) - p.GetDistance(r_temp_2[i], r_temp_2[i + 1]));
	//	//	//	//r_temp_2.erase(r_temp_2.begin() + i);
	//	//	//	//增量obj可能是0

	//	//	////double obj_delta = r_temp.m_obj- r_temp_2.m_obj;//对比插入r的i位置后目标函数，和现在路径的目标函数，（把该客户删除路径后）减少的obj
	//	//	//	if (obj_delta < min_obj)
	//	//	//	{
	//	//	//		min_obj = obj_delta;
	//	//	//		best_delete = i;	//找到一个影响最小的删除路径位置
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

	//	//加了一个超载的惩罚
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

	//1是第一个客户，r.size()-2是最后一个客户，r.size()-1是车库。
	//如果r.size==4,0-7-3-0,r[1]是第一个客户，r[2]是最后一个客户，
	//r[0]这里就是51；已经在前面编译好了
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