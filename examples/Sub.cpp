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






struct Csv2rec {
	vector<string> header;
	vector<double> xs;
	vector<vector<double> > data;

	Communities layer1, layer2;


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
		addline(line + 1);
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


	string graph_path;
	if (argc == 2)
		graph_path = argv[1];
	else
		graph_path = "F:/HICODE_SUB/syn/param/0.01/";

	string basealg;
	if (argc >= 3)
		basealg = argv[2];
	else
		basealg = "mod";

	int iterator_times = 1;
	if (argc >= 4)
		iterator_times = atoi(argv[3]);

	string outdir = "sub_" + basealg + "/";
	Communities::mkdir(outdir);

	string hicode_path = "hicode_" + basealg + "/";



	Graph g;
	g.load(graph_path + "graph");

	Communities layer1, layer1reduce;
	layer1.load(graph_path + hicode_path + "maxmodlayer1.gen");
	layer1reduce.load(graph_path + hicode_path + "maxmodlayer1.gen");
	Communities layer2, layer2reduce;
	layer2.load(graph_path + hicode_path + "maxmodlayer2.gen");
	layer2reduce.load(graph_path + hicode_path + "maxmodlayer2.gen");
	
	char buff[256];

	//记录每次迭代变化
	Csv2rec csv_last;
	csv_last.adddata(0);
	csv_last.adddata(0);
	csv_last.adddata(0);
	csv_last.adddata(0);

	for (int iter_i = 1; iter_i <= iterator_times; ++iter_i)
	{
		Communities sub1;
		for (size_t i = 0; i < layer1.size(); ++i)
		{
			if (layer1.comms[i].size() > 9)
			{
				Graph subg = g.getSubGraph(layer1.comms[i]);
				Communities subcs = subg.runMod();
				sub1.addCommunities(subcs);
			}
			else
			{
				Communities subcs;
				subcs.addCommunity(layer1.comms[i]);
				sub1.addCommunities(subcs);
			}

		}
		sprintf(buff, (outdir + "sub1_%d.gen").c_str(), iter_i);
		printf((outdir + "sub1_%d.gen").c_str(), iter_i);
		sub1.save(buff);


		Communities sub2;
		for (size_t i = 0; i < layer2.size(); ++i)
		{
			if (layer2.comms[i].size() > 9)
			{
				Graph subg = g.getSubGraph(layer2.comms[i]);
				Communities subcs = subg.runMod();
				sub2.addCommunities(subcs);
			}
			else
			{
				Communities subcs;
				subcs.addCommunity(layer2.comms[i]);
				sub2.addCommunities(subcs);
			}
		}
		sprintf(buff, (outdir + "sub2_%d.gen").c_str(), iter_i);
		printf((outdir + "sub2_%d.gen").c_str(), iter_i);
		sub2.save(buff);

		
			Graph g1 = g.reduceWeight(layer2reduce);
			Communities sub1_reduce;
			
			for (size_t i = 0; i < layer1reduce.size(); ++i)
			{
				if (layer1reduce.comms[i].size() > 9)
				{
					Graph subg = g1.getSubGraph(layer1reduce.comms[i]);
					Communities subcs = subg.runMod();
					sub1_reduce.addCommunities(subcs);
				}
				else
				{
					Communities subcs;
					subcs.addCommunity(layer1reduce.comms[i]);
					sub1_reduce.addCommunities(subcs);
				}
			}
			sprintf(buff, (outdir + "sub1_reduce_%d.gen").c_str(), iter_i);
			printf((outdir + "sub1_reduce_%d.gen").c_str(), iter_i);
			sub1_reduce.save(buff);
			

			Graph g2 = g.reduceWeight(layer1reduce);
			Communities sub2_reduce;
			
			for (size_t i = 0; i < layer2reduce.size(); ++i)
			{
				if (layer2reduce.comms[i].size() > 9)
				{
					Graph subg = g2.getSubGraph(layer2reduce.comms[i]);
					Communities subcs = subg.runMod();
					sub2_reduce.addCommunities(subcs);
				}
				else
				{
					Communities subcs;
					subcs.addCommunity(layer2reduce.comms[i]);
					sub2_reduce.addCommunities(subcs);
				}
			}
			sprintf(buff, (outdir + "sub2_reduce_%d.gen").c_str(), iter_i);
			printf((outdir + "sub2_reduce_%d.gen").c_str(), iter_i);
			sub2_reduce.save(buff);
			

		double nmi_last1, nmi_last2, nr1, nr2;
		nmi_last1 = layer1.calcNMI(sub1);
		nmi_last2 = layer2.calcNMI(sub2);
		nr1 = layer1.calcNMI(sub1_reduce);
		nr2 = layer2.calcNMI(sub2_reduce);
		layer1 = sub1;
		layer1reduce = sub1_reduce;
		layer2 = sub2;
		layer2reduce = sub2_reduce;
		csv_last.addline(iter_i);
		csv_last.adddata(nmi_last1);
		csv_last.adddata(nmi_last2);
		csv_last.adddata(nr1);
		csv_last.adddata(nr2);
		
	}

	csv_last.save((outdir + "nmi_last.txt").c_str());

	printf("------------\ndone\n");
	return 0;
}