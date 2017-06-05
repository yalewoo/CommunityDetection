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
	Graph::loadConfig("F:/Project/CommunityDetection/config.txt");

	Communities truth, truth1, truth2;
	truth.load("F:/HICODE_SUB/result/syn/truth.gen");
	truth1.load("F:/HICODE_SUB/result/syn/truth1.gen");
	truth2.load("F:/HICODE_SUB/result/syn/truth2.gen");

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

	double t1_nmi1, t1_nmi2, t2_nmi1, t2_nmi2;
	t1_nmi1 = layer1.calcNMI(truth1);
	t1_nmi2 = layer2.calcNMI(truth1);
	t2_nmi1 = layer1.calcNMI(truth2);
	t2_nmi1 = layer2.calcNMI(truth2);






	Csv2rec csv_mod;
	csv_mod.addheader("layer1");
	csv_mod.addheader("layer2");
	csv_mod.addline(0);
	csv_mod.adddata(mod1);
	csv_mod.adddata(mod2);

	Csv2rec csv_nmi_all_layer;
	csv_nmi_all_layer.addheader("layer1");
	csv_nmi_all_layer.addheader("layer2");
	csv_nmi_all_layer.addline(0);
	csv_nmi_all_layer.adddata(nmi1);
	csv_nmi_all_layer.adddata(nmi2);

	Csv2rec csv_nmi_truth1;
	csv_nmi_truth1.addheader("layer1");
	csv_nmi_truth1.addheader("layer2");
	csv_nmi_truth1.addline(0);
	csv_nmi_truth1.adddata(t1_nmi1);
	csv_nmi_truth1.adddata(t1_nmi2);
	Csv2rec csv_nmi_truth2;
	csv_nmi_truth2.addheader("layer1");
	csv_nmi_truth2.addheader("layer2");
	csv_nmi_truth2.addline(0);
	csv_nmi_truth2.adddata(t2_nmi1);
	csv_nmi_truth2.adddata(t2_nmi2);

	Csv2rec csv_last;
	csv_last.addheader("layer1");
	csv_last.addheader("layer2");
	csv_last.addline(0);
	csv_last.adddata(1);
	csv_last.adddata(1);

	int max_iter = 0;
	double max_mod = 0;
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
		csv_nmi_all_layer.addline(iter_i + 1);
		csv_nmi_all_layer.adddata(nmi1);
		csv_nmi_all_layer.adddata(nmi2);


		t1_nmi1 = layer1.calcNMI(truth1);
		t1_nmi2 = layer2.calcNMI(truth1);
		t2_nmi1 = layer1.calcNMI(truth2);
		t2_nmi1 = layer2.calcNMI(truth2);
		csv_nmi_truth1.addline(iter_i + 1);
		csv_nmi_truth1.adddata(t1_nmi1);
		csv_nmi_truth1.adddata(t1_nmi2);
		csv_nmi_truth2.addline(iter_i + 1);
		csv_nmi_truth2.adddata(t2_nmi1);
		csv_nmi_truth2.adddata(t2_nmi2);


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
	csv_nmi_all_layer.save("hicode/nmi_all_layer.txt");
	csv_last.save("hicode/nmi_last.txt");
	csv_nmi_truth1.save("hicode/nmi_truth1.txt");
	csv_nmi_truth2.save("hicode/nmi_truth2.txt");
	printf("------------\ndone\n");
	return 0;
}