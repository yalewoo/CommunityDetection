#include <cstdio>
#include <iostream>
#include "../Graph.h"

#include "../os.h"

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

	Communities detected;
	Communities truth;
	//detected.load("F:/Bio/20180227/hicode_mod_ReduceWeight_2layers/maxOriginlayer2.gen");
	//detected.load("F:/Bio/20180227/hicode_mod_ReduceWeight_2layers_ori_sub/sub2_1.gen");
	//detected.load("F:/Bio/20180103/newCommunity/reduceMethod/ReduceP/4/AllLayer.gen");
	detected.load("F:/Bio/20180227/truth/GOA2GO.gen");

	truth.load("F:/Bio/20180227/alllayer/alllayer.gen");

	FILE * fp = fopen("F:/Bio/20180227/truth/allf1.csv", "w");
	fprintf(fp, "i,size i,j,size j,f1\n");

	for (int i = 0; i < detected.size(); ++i)
	{
		double maxf1 = 0;
		int maxj = 0;
		for (int j = 0; j < truth.size(); ++j)
		{
			double f1 = Communities::f1(detected.comms[i], truth.comms[j]);
			if (f1 > maxf1)
			{
				maxf1 = f1;
				maxj = j;
			}
		}
		fprintf(fp, "%d,%d,%d,%d,%lf\n", i, detected.comms[i].size(),maxj, truth.comms[maxj].size(), maxf1);
	}
	fclose(fp);


	//return 0;




	fp = fopen("F:/Bio/20180227/truth/allJ.csv", "w");
	fprintf(fp, "i,size i,j,size j,join/union\n");
	for (int i = 0; i < detected.size(); ++i)
	{
		double maxf1 = 0;
		int maxj = 0;
		for (int j = 0; j < truth.size(); ++j)
		{
			double f1 = Communities::findSimilar(detected.comms[i], truth.comms[j]);
			if (f1 > maxf1)
			{
				maxf1 = f1;
				maxj = j;
			}
		}
		fprintf(fp, "%d,%d,%d,%d,%lf\n", i, detected.comms[i].size(), maxj, truth.comms[maxj].size(), maxf1);
	}
	fclose(fp);
	
	return 0;
}