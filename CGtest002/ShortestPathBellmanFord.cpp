#include "ShortestPathBellmanFord.h"


double ShortestPathBellmanFord(AdjMatrixDirNetwork &network, Route &route_rec_id, int startVex)
{
	//int count_access = 0;
	unsigned index = 0;
	
	//如果不add load相当于不采用trie
	//trie.add_load(r, 0, r_temp, max_delta_sum);
	double dist_rec;
	double *dist, *distTemp;
	int *path, i, j, k;
	int vexNum = network.GetVexNum(), infinity = network.GetInfinity();
	//cout << "vexNum=" << vexNum << endl;
	//dist用于记录到该点的最短距离
	dist = new double[vexNum];
	//path用于记录该点的上一个点的编号
	path = new int[vexNum];
	distTemp = new double[vexNum];




	path[0] = -1;
	dist[0] = 0;
	for (i = 1; i < vexNum; i++) {    //初始化
		dist[i] = network.GetCost(0, i);
		path[i] = 0;
	}



	//for (i = index; i < vexNum; i++) {    //初始化
	////这边只考虑起点是0的情况
	//	dist[i] = (i != 0) ? network.GetCost(0, i) : 0;
	//	//我们模型中起点能到任何点，所以直接:
	//	path[i] = 0;
	//	path[0] = -1;
	//}


	//for (i = 0; i < vexNum; i++)     //初始化
	//{
	//	//如果考虑trie，这里前面能查到的部分可以用trie读取
	//	//查询（network.GetElem(i)），如果能查询到最后一个i，给一个flag，说明完全匹配，然后直接delete & return
	//	//如果是起点，则到它的距离是-999.95，否则都是取起点到它权重
	//	dist[i] = (i != startVex) ? network.GetCost(startVex, i) : -999.95;
	//	//如果是起点或者起点到不了它，则前置点先设为-1；否则设为起点
	//	if (dist[i] == -999.95 || dist[i] == infinity)
	//		path[i] = -1;
	//	else
	//		path[i] = startVex;
	//
	//	if (i == startVex) {
	//		dist[i] = 0;
	//	}

	//	////如果是起点，则到它的距离是-999.95，否则都是取起点到它权重
	//	//dist[i] = (i != startVex) ? network.GetCost(startVex, i) : -999.95;
	//	////如果是起点或者起点到不了它，则前置点先设为-1；否则设为起点
	//	//if (dist[i] == -999.95 || dist[i] == infinity)
	//	//	path[i] = -1;
	//	//else
	//	//	path[i] = startVex;

	//	//if (i == startVex) {
	//	//	dist[i] = 0;
	//	//}
	//	//
	//}



	////如果考虑trie，这里前面能查到的部分可以跳过

	//for (j = 1; j < vexNum; j++) {
	//	for (k = network.FirstAdjVex(j, index - 1); k != -1; k = network.NextAdjVex(j, k)) {
	//		if (dist[j] + network.GetCost(j, k) < dist[k])
	//		{
	//			dist[k] = dist[j] + network.GetCost(j, k);
	//			path[k] = j;
	//		}

	//	}
	//}









	////方案2
	path[0] = -1;
	dist[0] = 0;
	vector<int> nodes_rec;
	for (i = 1; i < vexNum; i++) {
		//必须是infinity，不然第一轮无法更新新的点进到集合
		dist[i] = network.GetCost(0, i);
		path[i] = 0;
		nodes_rec.push_back(i);
	}
	while (nodes_rec.size() > 0)   //dist(2)到dist(n-1)；这个就是纯计数，n-2次计算。之所以是n-2，是因为之前初始化已经计算了一次了。一共计算n-1次
	{
		//改成while，只要集合不空

		//for (j = 0; j < vexNum; j++)
		//	distTemp[j] = dist[j];

		j = nodes_rec[0];
		nodes_rec.erase(nodes_rec.begin() + 0);
		//cout << "nodes_rec.size()=" << nodes_rec.size() << endl;
		for (k = network.FirstAdjVex(j, j); k != -1; k = network.NextAdjVex(j, k)) {
			if (dist[j] + network.GetCost(j, k) < dist[k])
			{
				dist[k] = dist[j] + network.GetCost(j, k);
				path[k] = j;
				nodes_rec.push_back(k);
				//记录到集合里
				//cout << "path[k]=" << path[k] << endl;
			}

		}
		sort(nodes_rec.begin(), nodes_rec.end());
		unique(nodes_rec.begin(), nodes_rec.end());


	}




	//////方案1
	//for (i = 2; i <= vexNum - 1; i++)   //dist(2)到dist(n-1)；这个就是纯计数，n-2次计算。之所以是n-2，是因为之前初始化已经计算了一次了。一共计算n-1次
	//{
	//	//如果考虑trie，这里前面能查到的部分可以跳过
	//	for (j = 0; j < vexNum; j++)
	//		distTemp[j] = dist[j];
	//	for (j = 0; j < vexNum; j++)
	//		if (j != startVex)
	//			//FirstAdjVex是从0点开始判断是否与j联通，可以改成只判断N点以后的.
	//			//从第一个联通的点开始，直到找不到联通的点（返回-1），每次都判断下不等式是否满足，不满足则更新；下一次再找
	//			//更新后面的点
	//			for (k = network.FirstAdjVex(j, index); k != -1; k = network.NextAdjVex(j, k)) {
	//				if (distTemp[j] + network.GetCost(j, k) < dist[k])
	//				{
	//					dist[k] = distTemp[j] + network.GetCost(j, k);
	//					path[k] = j;
	//				}

	//			}



	//}


	//	//for (j = 0; j < vexNum; j++)
	//	//	distTemp[j] = dist[j];
	//	//for (j = 0; j < vexNum; j++)
	//	//	if (j != startVex)
	//	//		//从第一个联通的点开始，直到找不到联通的点（返回-1），每次都判断下不等式是否满足，不满足则更新；下一次再找
	//	//		for (k = network.FirstAdjVex(j); k != -1; k = network.NextAdjVex(j, k))
	//	//			if (distTemp[j] + network.GetCost(j, k) < dist[k])
	//	//			{
	//	//				dist[k] = distTemp[j] + network.GetCost(j, k);
	//	//				path[k] = j;
	//	//			}
	//}

	route_rec_id.clear();
	j = vexNum - 1;
	//route_rec_id.push_back(101);
	//先补上r里的第一个
	//cout << "route_rec_id=" << endl;
	while (path[j] != -1)
	{
		j = path[j];
		//逆向输出
		route_rec_id.push_back( network.GetElem(j));
		//cout << network.GetElem(j) << ",";
	}


	//for (i = 0; i < vexNum; i++)
	//{
	//	if (i != startVex)
	//	{
	//		cout << "从起点" << network.GetElem(startVex) << "到终点" << network.GetElem(i) << "的最短路径长度为:" << dist[i];
	//		cout << "  最短路径为:" << network.GetElem(i);
	//		j = i;
	//		//从要到达的点逆着输出
	//		while (path[j] != -1)
	//		{
	//			j = path[j];
	//			//输出的是节点名称，char
	//			cout << "<-" << network.GetElem(j);
	//		}
	//		cout << endl;
	//	}
	//}

	
	

	////输出到每点的最短距离
	//cout << "输出到每点的最短距离: ";
	//for (i = 0; i < vexNum; i++) {	
	//	cout << dist[i] << " ";
	//}
	//cout << endl;
	////输出每个点的前置点
	//cout << "输出每个点的前置点: ";
	//for (i = 0; i < vexNum; i++) {	
	//	cout << path[i] << " ";
	//}	
	//cout << endl;

	dist_rec = dist[vexNum - 1];

	//上面没匹配到最后一个，则这里需要add；记录最后的route
	//如果不add load相当于不采用trie
  


	delete[] dist;
	delete[] path;
	delete[] distTemp;

	return dist_rec;
}