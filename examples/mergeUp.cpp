#include <cstdio>
#include <iostream>
#include "../Graph.h"

#include "../os.h"

using std::cout;
using std::endl;

map<string, string> Graph::config;






int main(int argc, char *argv[])
{
	Graph::loadConfig("F:/Project/CommunityDetection/config.txt");


	string graph_path;
	if (argc == 2)
		graph_path = argv[1];
	else
		graph_path = "F:/Bio/20180227/";

	string basealg;
	if (argc >= 3)
		basealg = argv[2];
	else
		basealg = "mod";

	int iterator_times = 1;
	if (argc >= 4)
		iterator_times = atoi(argv[3]);

	string outdir = "Merge_" + basealg + "/";
	os::mkdir(outdir);



	Graph g;
	g.load(graph_path + "graph");

	Communities layer1;
	layer1.load("F:/Bio/20180227/alllayer/maxOriginlayer1.gen");
	Communities layer2;
	layer2.load("F:/Bio/20180227/alllayer/maxOriginlayer2.gen");

	char buff[256];

	

	for (int iter_i = 0; iter_i < iterator_times; ++iter_i)
	{
		Communities merge1, merge2;
		

		
	}


	printf("------------\ndone\n");
	return 0;
}