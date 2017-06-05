#include <cstdio>
#include <iostream>
#include "../Graph.h"

#include "../draw.h"

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
	//读配置文件，该文件记录社团检测算法的具体路径
	Graph::loadConfig("F:/Project/CommunityDetection/config.txt");

	Communities truth;
	truth.load("F:/HICODE_SUB/result/syn/truth.gen");

	int iterator_times = 20;

	Graph g;
	g.load("F:/HICODE_SUB/result/syn/graph");
	Communities layer1, layer2;
	Communities layer1_last, layer2_last;

	layer1 = g.runMod();
	layer1_last = layer1;
	//layer1.print(true);

	Graph g2 = g.reduceWeight(layer1);

	layer2 = g2.runMod();
	layer2_last = layer2;

	double mod1, mod2;
	mod1 = layer1.calcModularity(g);
	mod2 = layer2.calcModularity(g);
	double nmi1, nmi2;
	nmi1 = layer1.calcNMI(truth);
	nmi2 = layer2.calcNMI(truth);

	Csv2rec csv_mod;
	csv_mod.addheader("layer1");
	csv_mod.addheader("layer2");
	csv_mod.addline(0);
	csv_mod.adddata(mod1);
	csv_mod.adddata(mod2);

	Csv2rec csv_nmi;
	csv_nmi.addheader("layer1");
	csv_nmi.addheader("layer2");
	csv_nmi.addline(0);
	csv_nmi.adddata(nmi1);
	csv_nmi.adddata(nmi2);

	Csv2rec csv_last;
	csv_last.addheader("layer1");
	csv_last.addheader("layer2");
	csv_last.addline(0);
	csv_last.adddata(1);
	csv_last.adddata(1);

	for (int iter_i = 0; iter_i < 20; ++iter_i)
	{
		Graph g3 = g.reduceWeight(layer2);

		layer1 = g3.runMod();

		Graph g4 = g.reduceWeight(layer1);

		layer2 = g4.runMod();

		mod1 = layer1.calcModularity(g);
		mod2 = layer2.calcModularity(g);
		csv_mod.addline(iter_i+1);
		csv_mod.adddata(mod1);
		csv_mod.adddata(mod2);

		nmi1 = layer1.calcNMI(truth);
		nmi2 = layer2.calcNMI(truth);
		csv_nmi.addline(iter_i + 1);
		csv_nmi.adddata(nmi1);
		csv_nmi.adddata(nmi2);


		double nmi_last1, nmi_last2;
		nmi_last1 = layer1.calcNMI(layer1_last);
		nmi_last2 = layer2.calcNMI(layer2_last);
		layer1_last = layer1;
		layer2_last = layer2;
		csv_last.addline(iter_i + 1);
		csv_last.adddata(nmi_last1);
		csv_last.adddata(nmi_last2);




		char buff[256];
		sprintf(buff, "hicode/layer1_%d.txt", iter_i);
		layer1.save(buff);

		sprintf(buff, "hicode/layer2_%d.txt", iter_i);
		layer2.save(buff);
	}

	csv_mod.save("hicode/mod.txt");
	csv_nmi.save("hicode/nmi.txt");
	csv_last.save("hicode/nmi_last.txt");
	printf("------------\ndone\n");
	return 0;
}