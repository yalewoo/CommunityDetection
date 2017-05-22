#include <cstdio>
#include <iostream>
#include "Graph.h"

using std::cout;
using std::endl;

map<string, string> Graph::config;


void showVector(vector<int> & v, string name = "vector", int id = 0)
{
	cout << name << " " << id << " = ";
	for (size_t i = 0; i < v.size(); ++i)
		cout << v[i] << " ";
	cout << endl;
}

int main()
{
	//读配置文件，该文件记录社团检测算法的具体路径
	Graph::loadConfig("F:/Project/CommunityDetection/config.txt");

	//Graph g(true);
	Graph g;
	//从edge list的文本文件读入图
	//g.load("F:/Project/CommunityDetection/graph.txt");
	//g.load("graph.txt");
	//g.load("graph.txt");

	//cout << "load graph ok" << endl;

	Communities cs;
	Communities cs2;

	// cs = g.runInfomap();
	// cs.calcModularity(g);
	// cs.print();
	// cs = g.runLinkComm();
	// cs.calcModularity(g);
	// cs.print();
	// cs = g.runOSLOM2();
	// cs.calcModularity(g);
	// cs.print();
	// cs = g.runGCE();
	// cs.calcModularity(g);
	// cs.print();
	// cs = g.runDemon();
	// cs.calcModularity(g);
	// cs.print();
	// cs = g.runCFinder();
	// cs.calcModularity(g);
	// cs.print();
	// cs = g.runMod();
	// cs.calcModularity(g);
	// cs.print();


	/*
	g.runInfomap().save("result/Infomap.gen");
	g.runLinkComm().save("result/LC.gen");
	g.runOSLOM2().save("result/OSLOM.gen");
	g.runGCE().save("result/GCE.gen");
	g.runDemon().save("result/Demon.gen");
	g.runCFinder().save("result/CFinder.gen");
	g.runMod().save("result/Mod.gen");

	/*
	g.showPic();
	Communities cs = g.runMod();
	g = g.remove(cs);
	g.print();
	g.showPic();*/
	//Communities cs;
	//cs.load("F:/HICODE_SUB/result/syn/Framework_ReduceP/maxLayer/Layer1.gen");
	//cs2.load("F:/HICODE_SUB/result/syn/truth.gen");
	//cout << "NMI(cs,cs2) = " << cs.calcNMI(cs2) << endl;
	//cout << "NMI(cs2,cs) = " << cs2.calcNMI(cs) << endl;

	//cs.print();
	//cs2.print();

	// for (int j = 0; j < cs2.comms.size(); ++j)
	// {
	// 	for (int i = 0; i < cs.comms.size(); ++i)
	// 	{
	// 		showVector(cs.comms[i].nodes);
	// 		showVector(cs2.comms[j].nodes);
	// 		cout << cs.H_Xi_given_Yj(cs.comms[i], cs2.comms[j]) << endl;
	// 		cout << cs.H_Xi_given_Yj(cs2.comms[j], cs.comms[i]) << endl;
	// 	}
	// }
	
	Communities detected;
	Communities truth;
	detected.load("F:/HICODE_SUB/result/syn/Framework_ReduceP/maxLayer/Layer1.gen");
	truth.load("F:/HICODE_SUB/result/syn/truth.gen");
	vector<int> v_index;
	vector<double> v_value;
	cout << Communities::Precision(detected, truth, v_index, v_value) << endl;
	for (size_t i = 0; i < v_index.size(); ++i)
	{
		cout << "--------Precision = " << v_value[i] << "--------" << endl;
		showVector(detected.comms[i].nodes, "detect", i);
		showVector(truth.comms[v_index[i]].nodes, "truth" , v_index[i]);
	}

	cout << Communities::Recall(detected, truth, v_index, v_value) << endl;
	for (size_t i = 0; i < v_index.size(); ++i)
	{
		cout << "--------Recall = " << v_value[i] << "--------" << endl;
		showVector(truth.comms[i].nodes, "truth", i);
		showVector(detected.comms[v_index[i]].nodes, "detected", v_index[i]);
	}
	
	cout << Communities::F1Score(detected, truth) << endl;

	//cout << g.calcModularity(cs) << endl;
	//cout << cs.calcModularity(g) << endl;

	//cs.print();

	printf("------------\ndone\n");
	return 0;
}