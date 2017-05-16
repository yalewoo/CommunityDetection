#include <cstdio>
#include "Graph.h"


int main()
{
	Graph g;
	g.loadConfig("F:/Project/CommunityDetection/config.txt");
	g.load("F:/Project/CommunityDetection/vs2015/vs2015/graph.txt");

	//g.runInfomap().print();
	//g.runLinkComm().print();
	//g.runOSLOM2().print();
	//g.runGCE().print();
	//g.runDemon().print();
	g.runCFinder().print();

	

	printf("done\n");
	return 0;
}