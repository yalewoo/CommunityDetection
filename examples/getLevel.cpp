#include <cstdio>
#include <iostream>
#include "../Graph.h"
#include "../Communities.h"

using std::cout;
using std::endl;

map<string, string> Graph::config;



int main(int argc, char *argv[])
{
	//读配置文件，该文件记录社团检测算法的具体路径
	Graph::loadConfig("F:/Project/CommunityDetection/config.txt");

	string outdir = "F:/Yun/Bio/fenceng/20180103/newCommunity/truth/";

	Graph g;
	g.load("F:/Yun/Bio/fenceng/20180103/newCommunity/graph/graph");

	Communities cs;
	cs.load(outdir + "GOA2GO.gen");

	g.print();

	cs.calcModularity(g);
	cs.print();

	vector<int> v(cs.size(), 0);
	vector<int> vp(cs.size(), -1);
	
	bool changed;
	for (;;)
	{
		changed = false;
		for (int i = 0; i < cs.size(); ++i)
		{
			Community & c1 = cs.comms[i];
			for (int j = 0; j < cs.size(); ++j)
			{
				if (j == i)
					continue;
				Community & c2 = cs.comms[j];
				if (c1.isSubsetOf(c2))
				{
					if (v[j] + 1 > v[i])
					{
						v[i] = v[j] + 1;
						vp[i] = j;
						changed = true;
					}

				}
			}
		}
		if (!changed)
			break;
	}

	int maxc = 0;
	for (int i = 0; i < v.size(); ++i)
	{
		if (v[i] > maxc)
			maxc = v[i];
		printf("community %d\t:\t%d : ", i,  v[i]);
		int p = vp[i];
		printf("%d", p);
		while (p != -1)
		{	
			p = vp[p];
			if (p == -1)
				break;
			printf(" -- %d", p);	


		}
		printf("\n");
	}

	for (int l = 0; l < maxc; ++l)
	{
		char name[256];
		sprintf(name, "t%d.gen", l);
		FILE * fp = fopen(name, "w");
		for (int i = 0; i < v.size(); ++i)
		{
			if (v[i] == l)
			{
				vector<int> & c = cs.comms[i].nodes;
				for (int j = 0; j < c.size(); ++j)
				{
					fprintf(fp, "%d ", c[j]);
				}
				fprintf(fp, "\n");
			}
		}
		fclose(fp);
	}
	
	


	printf("------------\ndone\n");
	return 0;
}