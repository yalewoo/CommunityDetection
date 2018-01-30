#include <cstdio>
#include <iostream>
#include "../Graph.h"
#include "../Config.h"


using std::cout;
using std::endl;

map<string, string> Graph::config;

template <typename T>
void showVector(vector<T> & v, string name = "vector", int id = 0)
{
	cout << name << " " << id << " (size=" << v.size() << ") : " << endl;
	for (size_t i = 0; i < v.size(); ++i)
		cout << v[i] << "\n";
	cout << endl;
}



#include "../os.h"

int main(int argc, char *argv[])
{
	Graph::loadConfig("F:/Project/CommunityDetection/config.txt");

	string path = "F:/Project/CommunityDetection/vs2015/vs2015/hicode_infomap_ReduceWeight_2layers/";
	Graph g;
	g.load(path + "graph1_1.graph");

	Communities cs;
	cs.load(path + "layer1_1.gen");

	g = g.reduceWeight(cs);

	g.save(path + "graph2_1_new.graph");
	

	

	printf("------------\ndone\n");
	return 0;
}