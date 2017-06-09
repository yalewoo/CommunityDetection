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




Communities layer1, layer2;

struct Csv2rec {
	vector<string> header;
	vector<double> xs;
	vector<vector<double> > data;


	int line = -1;

	Csv2rec() {
		header.push_back(string("x"));

		addheader("layer1");
		addheader("layer2");
		addline(0);

	}
	void init(Communities & truth)
	{
		double nmi1, nmi2;
		nmi1 = layer1.calcNMI(truth);
		nmi2 = layer2.calcNMI(truth);

		adddata(nmi1);
		adddata(nmi2);
	}
	void addNMI(int line, Communities & truth)
	{
		double nmi1 = layer1.calcNMI(truth);
		double nmi2 = layer2.calcNMI(truth);
		addline(line);
		adddata(nmi1);
		adddata(nmi2);
	}
	void addheader(char * s)
	{
		header.push_back(string(s));
	}
	void addline(double x)
	{
		xs.push_back(x);
		data.push_back(vector<double>());
		++line;
	}
	void adddata(double n)
	{
		data[line].push_back(n);
	}
	bool save(const char * fn)
	{
		FILE * fp = fopen(fn, "w");
		if (!fp) return false;



		size_t i;
		fprintf(fp, "%s", header[0].c_str());
		for (i = 1; i < header.size(); ++i)
		{
			fprintf(fp, ",%s", header[i].c_str());
		}
		fprintf(fp, "\n");


		for (i = 0; i < data.size(); ++i)
		{
			fprintf(fp, "%lf", xs[i]);
			for (size_t j = 0; j < data[i].size(); ++j)
			{
				fprintf(fp, ",%lf", data[i][j]);
			}
			fprintf(fp, "\n");
		}

		fclose(fp);

		return true;
	}
};



int main(int argc, char *argv[])
{
	Graph::loadConfig("F:/Project/CommunityDetection/config.txt");

	Communities truth, truth1, truth2;
	Communities truth1_1, truth1_2, truth2_1, truth2_2;

	string graph_path;
	if (argc == 2)
		graph_path = argv[1];
	else
		graph_path = "F:/HICODE_SUB/facebook/Harvard1/";

	Communities::mkdir("hicode/");

	truth.load(graph_path + "truth.gen");
	truth1.load(graph_path + "truth1.gen");
	truth2.load(graph_path + "truth2.gen");

	
	truth1_1.load(graph_path + "truth1_1.gen");
	truth1_2.load(graph_path + "truth1_2.gen");
	truth2_1.load(graph_path + "truth2_1.gen");
	truth2_2.load(graph_path + "truth2_2.gen");
	

	int iterator_times = 20;

	Graph g;
	g.load(graph_path + "graph");
	
	Communities layer1_last, layer2_last;

	layer1 = g.runMod();
	layer1_last = layer1;
	//layer1.print(true);

	Graph g2 = g.reduceWeight(layer1);
	g2.save("hicode/layer2_0.graph");

	layer2 = g2.runMod();
	layer2_last = layer2;

	layer1.save("hicode/layer1_0.txt");
	layer2.save("hicode/layer2_0.txt");

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


	//记录每次迭代的modularity值
	Csv2rec csv_mod;
	csv_mod.adddata(mod1);
	csv_mod.adddata(mod2);

	//记录每次迭代结果和truth比较
	Csv2rec csv_nmi_truth;
	csv_nmi_truth.init(truth);
	Csv2rec csv_nmi_truth1;
	csv_nmi_truth1.init(truth1);
	Csv2rec csv_nmi_truth2;
	csv_nmi_truth2.init(truth2);

	Csv2rec csv_nmi_truth1_1;
	csv_nmi_truth1_1.init(truth1_1);
	Csv2rec csv_nmi_truth1_2;
	csv_nmi_truth1_2.init(truth1_2);
	Csv2rec csv_nmi_truth2_1;
	csv_nmi_truth2_1.init(truth2_1);
	Csv2rec csv_nmi_truth2_2;
	csv_nmi_truth2_2.init(truth2_2);


	//记录每次迭代变化
	Csv2rec csv_last;
	csv_last.adddata(1);
	csv_last.adddata(1);

	int max_iter = 0;
	double max_mod = 0;
	Communities max_layer1, max_layer2;

	for (int iter_i = 1; iter_i <= iterator_times; ++iter_i)
	{
		Graph g3 = g.reduceWeight(layer2);

		layer1 = g3.runMod();

		Graph g4 = g.reduceWeight(layer1);

		layer2 = g4.runMod();

		mod1 = layer1.calcModularity(g);
		mod2 = layer2.calcModularity(g);
		csv_mod.addline(iter_i);
		csv_mod.adddata(mod1);
		csv_mod.adddata(mod2);

		if (mod1 + mod2 > max_mod)
		{
			max_mod = mod1 + mod2;
			max_iter = iter_i;
			max_layer1 = layer1;
			max_layer2 = layer2;
		}


		csv_nmi_truth.addNMI(iter_i, truth);
		csv_nmi_truth1.addNMI(iter_i, truth1);
		csv_nmi_truth2.addNMI(iter_i, truth2);

		csv_nmi_truth1_1.addNMI(iter_i, truth1_1);
		csv_nmi_truth1_2.addNMI(iter_i, truth1_2);
		csv_nmi_truth2_1.addNMI(iter_i, truth2_1);
		csv_nmi_truth2_2.addNMI(iter_i, truth2_2);




		double nmi_last1, nmi_last2;
		nmi_last1 = layer1.calcNMI(layer1_last);
		nmi_last2 = layer2.calcNMI(layer2_last);
		layer1_last = layer1;
		layer2_last = layer2;
		csv_last.addline(iter_i);
		csv_last.adddata(nmi_last1);
		csv_last.adddata(nmi_last2);




		char buff[256];
		sprintf(buff, "hicode/layer1_%d.txt", iter_i);
		layer1.save(buff);

		sprintf(buff, "hicode/layer2_%d.txt", iter_i);
		layer2.save(buff);
	}

	csv_mod.save("hicode/mod.txt");
	csv_nmi_truth.save("hicode/nmi_truth.txt");
	csv_last.save("hicode/nmi_last.txt");
	csv_nmi_truth1.save("hicode/nmi_truth1.txt");
	csv_nmi_truth2.save("hicode/nmi_truth2.txt");
	csv_nmi_truth1_1.save("hicode/nmi_truth1_1.txt");
	csv_nmi_truth1_2.save("hicode/nmi_truth1_2.txt");
	csv_nmi_truth2_1.save("hicode/nmi_truth2_1.txt");
	csv_nmi_truth2_2.save("hicode/nmi_truth2_2.txt");


	layer1.save("hicode/maxmodlayer1.gen");
	layer2.save("hicode/maxmodlayer2.gen");
	FILE * fp = fopen("hicode/maxmod.txt", "w");
	fprintf(fp, "%d", max_iter);
	fclose(fp);

	printf("------------\ndone\n");
	return 0;
}