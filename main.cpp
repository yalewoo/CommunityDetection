#include <cstdio>
#include <iostream>
#include "Graph.h"

using std::cout;
using std::endl;

map<string, string> Graph::config;


void showVector(vector<int> & v, string name = "vector", int id = 0)
{
	cout << name << " " << id << " (size=" << v.size() << ") : " << endl;
	//for (size_t i = 0; i < v.size(); ++i)
		//cout << v[i] << " ";
	//cout << endl;
}

int main(int argc, char *argv[])
{
	//读配置文件，该文件记录社团检测算法的具体路径
	Graph::loadConfig("F:/Project/CommunityDetection/config.txt");



	Graph g;
	g.load("F:/HICODE_SUB/result/syn/graph");
	//g.print();



	Communities truth, truth1, truth2;
	Communities truth1_1, truth1_2, truth2_1, truth2_2;
	truth.load("F:/HICODE_SUB/result/syn/truth.gen");
	truth1.load("F:/HICODE_SUB/result/syn/truth1.gen");
	truth2.load("F:/HICODE_SUB/result/syn/truth2.gen");

	truth1_1.load("F:/HICODE_SUB/result/syn/truth1_1.gen");
	truth1_2.load("F:/HICODE_SUB/result/syn/truth1_2.gen");
	truth2_1.load("F:/HICODE_SUB/result/syn/truth2_1.gen");
	truth2_2.load("F:/HICODE_SUB/result/syn/truth2_2.gen");

	Communities layer1, layer2;
	layer1.load("F:/Project/CommunityDetection/vs2015/vs2015/hicode/layer1_4.txt");
	layer2.load("F:/Project/CommunityDetection/vs2015/vs2015/hicode/layer2_4.txt");


	double Q1 = truth1.calcModularity(g);
	double Q2 = truth2.calcModularity(g);
	cout << "Q1 = " << Q1 << endl;
	cout << "Q2 = " << Q2 << endl;

	cout << "NMI = " << truth1.calcNMI(truth2) << endl;

	cout << "NMI(layer1, truth) = " << layer1.calcNMI(truth) << endl;
	cout << "NMI(layer1, truth1) = " << layer1.calcNMI(truth1) << endl;
	cout << "NMI(layer1, truth2) = " << layer1.calcNMI(truth2) << endl;
	cout << "NMI(layer1, truth1_1) = " << layer1.calcNMI(truth1_1) << endl;
	cout << "NMI(layer1, truth1_2) = " << layer1.calcNMI(truth1_2) << endl;
	cout << "NMI(layer1, truth2_1) = " << layer1.calcNMI(truth2_1) << endl;
	cout << "NMI(layer1, truth2_2) = " << layer1.calcNMI(truth2_2) << endl;


	cout << "NMI(layer2, truth) = " << layer2.calcNMI(truth) << endl;
	cout << "NMI(layer2, truth1) = " << layer2.calcNMI(truth1) << endl;
	cout << "NMI(layer2, truth2) = " << layer2.calcNMI(truth2) << endl;
	cout << "NMI(layer2, truth1_1) = " << layer2.calcNMI(truth1_1) << endl;
	cout << "NMI(layer2, truth1_2) = " << layer2.calcNMI(truth1_2) << endl;
	cout << "NMI(layer2, truth2_1) = " << layer2.calcNMI(truth2_1) << endl;
	cout << "NMI(layer2, truth2_2) = " << layer2.calcNMI(truth2_2) << endl;


	printf("------------\ndone\n");
	return 0;
}