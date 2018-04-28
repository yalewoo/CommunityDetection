
#include <set>
#include <ctime>
#include <vector>
#include <iostream>
#include <unordered_set>

#include <cstdlib>
#include <cstdio>
using namespace std; 

#include "../Graph.h"
#include "../Config.h"

extern Config hicode_config;

void rand_inplace(vector<int>& arr)
{
	int len = arr.size();
	for (int i = 0; i<len; ++i)
		swap(arr[i], arr[rand() % (len - i) + i]);
}


struct Param {
	int nodes;
	int n11;
	int n12;
	int n21;
	int n22;

	double p11;
	double p12;
	double p21;
	double p22;
};

string output = "F:/HICODE_SUB/0426new/";
void generateGraph(Param param)
{
	int NODE_NUM = param.nodes;
	int n11 = param.n11;
	int n12 = param.n12;
	int n21 = param.n21;
	int n22 = param.n22;
	double p11 = param.p11;
	double p12 = param.p12;
	double p21 = param.p21;
	double p22 = param.p22;


	vector<vector<int>> matrix(NODE_NUM, vector<int>(NODE_NUM, 0));

	Communities L11, L12, L21, L22;

	vector<int> arr(NODE_NUM);
	for (int i = 0; i<NODE_NUM; ++i)
		arr[i] = i;

	srand((unsigned)time(NULL));
	rand_inplace(arr);

	int n;
	
	
	//第一层根
	for (int k = 0; k < n11; ++k)
	{
		//社团k的结点个数
		int size = NODE_NUM / n11;

		int edge = p11 * size * (size - 1) / 2;


		Community c;
		for (int i = k * size; i < (k + 1) * size; ++i)
			c.add(arr[i]);

		while (edge > 0)
		{
			int im = rand() % (size)+ k * size;
			int in = rand() % (size)+ k * size;

			if (im != in && matrix[im][in] == 0)
			{
				matrix[im][in] = 1;
				matrix[in][im] = 1;
				--edge;
			}
		}

		L11.addCommunity(c);
	}
	Graph g11;
	g11.loadMatrix(matrix);
	g11.save(output + "g11.txt");
	L11.save(output + "L11.txt");

	//第一层叶子
	for (int k = 0; k < n12; ++k)
	{
		//社团k的结点个数
		int size = NODE_NUM / n12;

		int edge = p12 * size * (size - 1) / 2;


		Community c;
		for (int i = k * size; i < (k + 1) * size; ++i)
			c.add(arr[i]);

		while (edge > 0)
		{
			int im = rand() % (size)+k * size;
			int in = rand() % (size)+k * size;

			if (im != in && matrix[im][in] == 0)
			{
				matrix[im][in] = 1;
				matrix[in][im] = 1;
				--edge;
			}
		}

		L12.addCommunity(c);
	}
	Graph g12;
	g12.loadMatrix(matrix);
	g12.save(output + "g12.txt");
	L12.save(output + "L12.txt");


	

	vector<vector<int>> matrix2(NODE_NUM, vector<int>(NODE_NUM, 0));

	for (int i = 0; i < NODE_NUM; ++i)
	{
		for (int j = 0; j < NODE_NUM; ++j)
		{
			if (matrix[i][j] == 1)
			{
				matrix2[arr[i]][arr[j]] = 1;
			}
		}
	}
	matrix = matrix2;

	Graph g13;
	g13.loadMatrix(matrix);
	g13.save(output + "g13.txt");


	//第二层根
	for (int k = 0; k < n21; ++k)
	{
		//社团k的结点个数
		int size = NODE_NUM / n21;

		int edge = p21 * size * (size - 1) / 2;


		Community c;
		for (int i = k * size; i < (k + 1) * size; ++i)
			c.add(i);

		while (edge > 0)
		{
			int im = rand() % (size)+k * size;
			int in = rand() % (size)+k * size;

			if (im != in && matrix[im][in] == 0)
			{
				matrix[im][in] = 1;
				matrix[in][im] = 1;
				--edge;
			}
		}

		L21.addCommunity(c);
	}
	Graph g21;
	g21.loadMatrix(matrix);
	g21.save(output + "g21.txt");
	L21.save(output + "L21.txt");

	//第二层叶子
	for (int k = 0; k < n22; ++k)
	{
		//社团k的结点个数
		int size = NODE_NUM / n22;

		int edge = p22 * size * (size - 1) / 2;


		Community c;
		for (int i = k * size; i < (k + 1) * size; ++i)
			c.add(i);

		while (edge > 0)
		{
			int im = rand() % (size)+k * size;
			int in = rand() % (size)+k * size;

			if (im != in && matrix[im][in] == 0)
			{
				matrix[im][in] = 1;
				matrix[in][im] = 1;
				--edge;
			}
		}

		L22.addCommunity(c);
	}
	Graph g22;
	g22.loadMatrix(matrix);
	g22.save(output + "g22.txt");
	L22.save(output + "L22.txt");


}


int main_generateGraph(int argc, char *argv[])
{


	Param param;
	param.nodes = 360;
	param.n11 = 3;
	param.n12 = 6;
	param.n21 = 4;
	param.n22 = 12;
	param.p11 = 0.2;
	param.p12 = 0.3;
	param.p21 = 0.15;
	param.p22 = 0.25;

	generateGraph(param);

	return 0;
}