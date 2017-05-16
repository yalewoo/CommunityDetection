#include <cstdio>
#include "Graph.h"

map<string, string> Graph::config;

int main()
{
	Graph::loadConfig("F:/Project/CommunityDetection/config.txt");

	Graph g;
	g.load("F:/Project/CommunityDetection/graph.txt");

	//g.runInfomap().print();
	//g.runLinkComm().print();
	//g.runOSLOM2().print();
	//g.runGCE().print();
	//g.runDemon().print();
	//g.runCFinder().print();
	//g.runMod().print();

	/*
	g.runInfomap().save("result/Infomap.gen");
	g.runLinkComm().save("result/LC.gen");
	g.runOSLOM2().save("result/OSLOM.gen");
	g.runGCE().save("result/GCE.gen");
	g.runDemon().save("result/Demon.gen");
	g.runCFinder().save("result/CFinder.gen");
	g.runMod().save("result/Mod.gen");
	*/

	//g.print();
	g.showPic();
	Communities cs = g.runMod();
	g = g.remove(cs);
	g.print();
	g.showPic();

	printf("------------\ndone\n");
	return 0;
}