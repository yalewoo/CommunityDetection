#include <cstdio>
#include "Graph.h"


int main()
{
	Graph g;
	g.loadConfig("F:/Project/CommunityDetection/vs2015/vs2015/config.txt");
	g.load("F:/Project/CommunityDetection/vs2015/vs2015/graph.txt");
	g.runOSLOM2().print();

	

	printf("done\n");
	return 0;
}