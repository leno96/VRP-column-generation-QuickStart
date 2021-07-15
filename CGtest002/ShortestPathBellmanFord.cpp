#include "ShortestPathBellmanFord.h"


double ShortestPathBellmanFord(AdjMatrixDirNetwork &network, Route &route_rec_id, int startVex)
{
	//int count_access = 0;
	unsigned index = 0;
	
	//�����add load�൱�ڲ�����trie
	//trie.add_load(r, 0, r_temp, max_delta_sum);
	double dist_rec;
	double *dist, *distTemp;
	int *path, i, j, k;
	int vexNum = network.GetVexNum(), infinity = network.GetInfinity();
	//cout << "vexNum=" << vexNum << endl;
	//dist���ڼ�¼���õ����̾���
	dist = new double[vexNum];
	//path���ڼ�¼�õ����һ����ı��
	path = new int[vexNum];
	distTemp = new double[vexNum];




	path[0] = -1;
	dist[0] = 0;
	for (i = 1; i < vexNum; i++) {    //��ʼ��
		dist[i] = network.GetCost(0, i);
		path[i] = 0;
	}



	//for (i = index; i < vexNum; i++) {    //��ʼ��
	////���ֻ���������0�����
	//	dist[i] = (i != 0) ? network.GetCost(0, i) : 0;
	//	//����ģ��������ܵ��κε㣬����ֱ��:
	//	path[i] = 0;
	//	path[0] = -1;
	//}


	//for (i = 0; i < vexNum; i++)     //��ʼ��
	//{
	//	//�������trie������ǰ���ܲ鵽�Ĳ��ֿ�����trie��ȡ
	//	//��ѯ��network.GetElem(i)��������ܲ�ѯ�����һ��i����һ��flag��˵����ȫƥ�䣬Ȼ��ֱ��delete & return
	//	//�������㣬�����ľ�����-999.95��������ȡ��㵽��Ȩ��
	//	dist[i] = (i != startVex) ? network.GetCost(startVex, i) : -999.95;
	//	//�������������㵽����������ǰ�õ�����Ϊ-1��������Ϊ���
	//	if (dist[i] == -999.95 || dist[i] == infinity)
	//		path[i] = -1;
	//	else
	//		path[i] = startVex;
	//
	//	if (i == startVex) {
	//		dist[i] = 0;
	//	}

	//	////�������㣬�����ľ�����-999.95��������ȡ��㵽��Ȩ��
	//	//dist[i] = (i != startVex) ? network.GetCost(startVex, i) : -999.95;
	//	////�������������㵽����������ǰ�õ�����Ϊ-1��������Ϊ���
	//	//if (dist[i] == -999.95 || dist[i] == infinity)
	//	//	path[i] = -1;
	//	//else
	//	//	path[i] = startVex;

	//	//if (i == startVex) {
	//	//	dist[i] = 0;
	//	//}
	//	//
	//}



	////�������trie������ǰ���ܲ鵽�Ĳ��ֿ�������

	//for (j = 1; j < vexNum; j++) {
	//	for (k = network.FirstAdjVex(j, index - 1); k != -1; k = network.NextAdjVex(j, k)) {
	//		if (dist[j] + network.GetCost(j, k) < dist[k])
	//		{
	//			dist[k] = dist[j] + network.GetCost(j, k);
	//			path[k] = j;
	//		}

	//	}
	//}









	////����2
	path[0] = -1;
	dist[0] = 0;
	vector<int> nodes_rec;
	for (i = 1; i < vexNum; i++) {
		//������infinity����Ȼ��һ���޷������µĵ��������
		dist[i] = network.GetCost(0, i);
		path[i] = 0;
		nodes_rec.push_back(i);
	}
	while (nodes_rec.size() > 0)   //dist(2)��dist(n-1)��������Ǵ�������n-2�μ��㡣֮������n-2������Ϊ֮ǰ��ʼ���Ѿ�������һ���ˡ�һ������n-1��
	{
		//�ĳ�while��ֻҪ���ϲ���

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
				//��¼��������
				//cout << "path[k]=" << path[k] << endl;
			}

		}
		sort(nodes_rec.begin(), nodes_rec.end());
		unique(nodes_rec.begin(), nodes_rec.end());


	}




	//////����1
	//for (i = 2; i <= vexNum - 1; i++)   //dist(2)��dist(n-1)��������Ǵ�������n-2�μ��㡣֮������n-2������Ϊ֮ǰ��ʼ���Ѿ�������һ���ˡ�һ������n-1��
	//{
	//	//�������trie������ǰ���ܲ鵽�Ĳ��ֿ�������
	//	for (j = 0; j < vexNum; j++)
	//		distTemp[j] = dist[j];
	//	for (j = 0; j < vexNum; j++)
	//		if (j != startVex)
	//			//FirstAdjVex�Ǵ�0�㿪ʼ�ж��Ƿ���j��ͨ�����Ըĳ�ֻ�ж�N���Ժ��.
	//			//�ӵ�һ����ͨ�ĵ㿪ʼ��ֱ���Ҳ�����ͨ�ĵ㣨����-1����ÿ�ζ��ж��²���ʽ�Ƿ����㣬����������£���һ������
	//			//���º���ĵ�
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
	//	//		//�ӵ�һ����ͨ�ĵ㿪ʼ��ֱ���Ҳ�����ͨ�ĵ㣨����-1����ÿ�ζ��ж��²���ʽ�Ƿ����㣬����������£���һ������
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
	//�Ȳ���r��ĵ�һ��
	//cout << "route_rec_id=" << endl;
	while (path[j] != -1)
	{
		j = path[j];
		//�������
		route_rec_id.push_back( network.GetElem(j));
		//cout << network.GetElem(j) << ",";
	}


	//for (i = 0; i < vexNum; i++)
	//{
	//	if (i != startVex)
	//	{
	//		cout << "�����" << network.GetElem(startVex) << "���յ�" << network.GetElem(i) << "�����·������Ϊ:" << dist[i];
	//		cout << "  ���·��Ϊ:" << network.GetElem(i);
	//		j = i;
	//		//��Ҫ����ĵ��������
	//		while (path[j] != -1)
	//		{
	//			j = path[j];
	//			//������ǽڵ����ƣ�char
	//			cout << "<-" << network.GetElem(j);
	//		}
	//		cout << endl;
	//	}
	//}

	
	

	////�����ÿ�����̾���
	//cout << "�����ÿ�����̾���: ";
	//for (i = 0; i < vexNum; i++) {	
	//	cout << dist[i] << " ";
	//}
	//cout << endl;
	////���ÿ�����ǰ�õ�
	//cout << "���ÿ�����ǰ�õ�: ";
	//for (i = 0; i < vexNum; i++) {	
	//	cout << path[i] << " ";
	//}	
	//cout << endl;

	dist_rec = dist[vexNum - 1];

	//����ûƥ�䵽���һ������������Ҫadd����¼����route
	//�����add load�൱�ڲ�����trie
  


	delete[] dist;
	delete[] path;
	delete[] distTemp;

	return dist_rec;
}