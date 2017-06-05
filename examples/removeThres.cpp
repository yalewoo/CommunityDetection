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
	Graph g;
	g.load("F:/Local/VGG/conv4_3.graph");
	g.print();
	Graph g1 = g.removeEdgeLessThan(0.75);
	g1.save("F:/Local/VGG/conv4_3_thres_0_75.graph");
	return 0;

	Communities truth1;
	truth1.load("F:/Local/VGG/VGG_GrounTruth1.txt");
	truth1.calcModularity(g);
	truth1.print();
	Communities truth2;
	truth2.load("F:/Local/VGG/VGG_GrounTruth2.txt");
	truth2.calcModularity(g);
	truth2.print();

	FILE * out = fopen("F:/Local/VGG/conv2_2_modularity.txt","w");
	fprintf(out, "thres, truth1, truth2, R1, R2\n");
	//double thres = 0.95;
	for (double thres = 0.0; thres < 1; thres += 0.05)
	{
		printf("Thres: %lf\n", thres);
		Graph g2 = g.removeEdgeLessThan(thres);
		printf("%d edges\n", g2.getEdgeNum());
		double m1, m2;
		m1 = truth1.calcModularity(g2);
		m2 = truth2.calcModularity(g2);
		cout << "Truth1 Modularity : " << m1 << endl;
		cout << "Truth2 Modularity : " << m2 << endl;

		Graph g3 = g2.reduceWeight(truth1);
		double m3, m4;
		m3 = truth1.calcModularity(g3);
		m4 = truth2.calcModularity(g3);
		cout << "Reduced Truth1 : " << m3 << endl;
		cout << "Reduced Truth2 : " << m4 << endl;
		fprintf(out, "%.2lf, %lf, %lf, %lf, %lf\n", thres, m1, m2, m3, m4);
	}


	return 0;
}