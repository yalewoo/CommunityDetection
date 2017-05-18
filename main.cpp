#include <cstdio>
#include <iostream>
#include "Graph.h"

using std::cout;
using std::endl;

map<string, string> Graph::config;

int main()
{
	//读配置文件，该文件记录社团检测算法的具体路径
	Graph::loadConfig("F:/Project/CommunityDetection/config.txt");

	//Graph g(true);
	Graph g;
	//从edge list的文本文件读入图
	//g.load("F:/Project/CommunityDetection/graph.txt");
	//g.load("graph.txt");
	g.load("graph.txt");

	cout << "load graph ok" << endl;

	Communities cs;

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
	cs.load("comm2.txt");

	cout << g.calcModularity(cs) << endl;
	cout << cs.calcModularity(g) << endl;

	//cs.print();

	printf("------------\ndone\n");
	return 0;
}