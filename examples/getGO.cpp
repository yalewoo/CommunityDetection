#include <cstdio>
#include <iostream>
#include "../Graph.h"
#include "../Communities.h"

#include <fstream>
#include <string>
#include <sstream>

using std::cout;
using std::endl;
using std::string;
using std::ifstream;
using std::stringstream;

map<string, string> Graph::config;



int main(int argc, char *argv[])
{
	//读配置文件，该文件记录社团检测算法的具体路径
	Graph::loadConfig("F:/Project/CommunityDetection/config.txt");

	string outdir = "F:/Yun/Bio/fenceng/20180103/newCommunity/reduceMethod/ReduceP/2/";

	Graph g;
	g.load("F:/Yun/Bio/fenceng/20180103/newCommunity/graph/graph");

	Communities layer1;
	layer1.load(outdir + "Layer1.gen");

	Communities layer2;
	layer2.load(outdir + "Layer2.gen");

	Communities go;
	go.load(outdir + "go_ids.gen");

	go.print();

	vector<string> t_id_go;
	ifstream f(outdir+"GO2.txt", std::ios::in);

	while (!f.eof())
	{
		string s;
		getline(f, s);

		//cout << s << endl;
		t_id_go.push_back(s);
	}
	f.close();

	FILE * fp = fopen((outdir + "layer1_GO.txt").c_str(), "w");
	for (int i = 0; i < layer1.size(); ++i)
	{
		Community & c = layer1.comms[i];
		printf("%d ", c.size());
		for (int j = 0; j < go.size(); ++j)
		{
			Community & c2 = go.comms[j];
			if (c2.size() == 2)
				//printf("%d ", j);
			if (c2.isSubsetOf(c))
			{
				fprintf(fp, "%s ", t_id_go[j].c_str());
				printf("%s ", t_id_go[j].c_str());
			}
		}
		fprintf(fp, "\n");
		printf("\n");
	}
	fclose(fp);

	fp = fopen((outdir + "layer2_GO.txt").c_str(), "w");
	for (int i = 0; i < layer2.size(); ++i)
	{
		Community & c = layer2.comms[i];
		printf("%d ", c.size());
		for (int j = 0; j < go.size(); ++j)
		{
			Community & c2 = go.comms[j];
			if (c2.size() == 2)
				//printf("%d ", j);
				if (c2.isSubsetOf(c))
				{
					fprintf(fp, "%s ", t_id_go[j].c_str());
					printf("%s ", t_id_go[j].c_str());
				}
		}
		fprintf(fp, "\n");
		printf("\n");
	}
	fclose(fp);

	printf("------------\ndone\n");
	return 0;
}