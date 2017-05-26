#include <cstdio>
#include <iostream>
#include "../Graph.h"

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
	if (argc != 5)
	{
		printf("ustage: f1fj graph detected.gen truth.gen outdir\n");
		return 0;
	}
	Communities detected;
	Communities truth;
	detected.load(argv[2]);
	truth.load(argv[3]);
	detected.removeSmallComm(2);
	truth.removeSmallComm(2);
	//detected.print();
	vector<int> v_index;
	vector<double> v_value;
	string outdir = argv[4];

	Communities::mkdir(outdir);

	printf("calculating Jaccard F1 Score...\n");
	pair<double, double> res;
	res = Communities::JaccardF1Score(detected, truth, outdir);
	cout << "Weighted Jaccard F1 = " << res.first << endl;
	cout << "UnWeighted Jaccard F1 = " << res.second << endl;
	printf("calculating Jaccard F1 Score done.\n");

	printf("calculating F1 Score...\n");
	res = Communities::F1Score(detected, truth, outdir);
	cout << "Weighted F1 = " << res.first << endl;
	cout << "UnWeighted F1 = " << res.second << endl;
	printf("calculating F1 Score done.\n");
	
	printf("calculating NMI Score...\n");
	cout << "NMI() = " << detected.calcNMI(truth,outdir) << endl;
	printf("calculating NMI Score done.\n");

	Graph g;
	g.load(argv[1]);
	double mod = g.calcModularity(detected);
	double modT = g.calcModularity(truth);
	string fn = outdir + "Modularity.csv";
	FILE * fp = fopen(fn.c_str(), "w");
	fprintf(fp, "Modlarity, Truth Modlarity\n");
	fprintf(fp, "%lf,%lf", mod, modT);
	fclose(fp);

	printf("------------\ndone\n");
	return 0;
}