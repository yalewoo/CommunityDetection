#include <cstdio>
#include <iostream>
#include "../Graph.h"
#include "../Config.h"
#include "../CsvOut.h"
#include "../os.h"

using std::cout;
using std::endl;

map<string, string> Graph::config;

template <typename T>
void showVector(vector<T> & v, string name = "vector", int id = 0)
{
	cout << name << " " << id << " (size=" << v.size() << ") : " << endl;
	//for (size_t i = 0; i < v.size(); ++i)
	//cout << v[i] << " ";
	//cout << endl;
}




Communities layer1, layer2;





int main(int argc, char *argv[])
{



	Graph::loadConfig("F:/Project/CommunityDetection/config.txt");

	Communities truth1, truth2;
	Communities truth1_1, truth1_2, truth2_1, truth2_2;

	string graph_path;
	if (argc >= 2)
		graph_path = argv[1];
	else
		graph_path = "F:/HICODE_SUB/syn/testGraph/";

	Graph g;
	g.load(graph_path + "graph");

	Config hicode_config;
	hicode_config.updateConfig("F:/Project/CommunityDetection/hicode_default.config");
	hicode_config.updateConfig(graph_path + "hicode.config");

	vector<string> vbasealg = hicode_config.getValue("base_alg");
	vector<string> vreduce_method = hicode_config.getValue("reduce_method");
	int iterator_times = atoi(hicode_config["number_of_iteration"].c_str());
	string nlayer = hicode_config["number_of_layers"];
	int layer_num = atoi(nlayer.c_str());
	vector<string> struth = hicode_config.getValue("truth");
	vector<Communities> truth;
	for (size_t i = 0; i < struth.size(); ++i)
	{
		string path = graph_path + struth[i];
		Communities t;
		t.load(path);
		truth.push_back(t);
	}



	for (int vbasealg_iter = 0; vbasealg_iter < vbasealg.size(); ++vbasealg_iter)
	{
		string basealg = vbasealg[vbasealg_iter];

		for (int reduce_method_iter = 0; reduce_method_iter < vreduce_method.size(); ++reduce_method_iter)
		{
			string reduce_method = vreduce_method[reduce_method_iter];

			string outdir = "hicode_" + basealg + "_" + reduce_method + "_"
				+ nlayer + "layers" + "/";

			os::mkdir(outdir);

			vector<Communities> layer;
			vector<Communities> layer_last;
			Graph g2 = g;

			vector<vector<double> > mods_ori_graph;
			vector<vector<double> > mods_reduce_graph;
			vector<vector<double> > nmi_last;
			vector<vector<vector<double> > > nmi_truth(truth.size(), vector<vector<double> >());

			//Stage 1 : Identification
			if (hicode_config["log_modularity"] == "true")
			{
				mods_ori_graph.push_back(vector<double>(layer_num));
				mods_reduce_graph.push_back(vector<double>(layer_num));
			}
			for (size_t t = 0; t < truth.size(); ++t)
			{
				nmi_truth[t].push_back(vector<double>(layer_num));
			}
			for (int i = 0; i < layer_num; ++i)
			{
				Graph g3 = g2;
				layer.push_back(g2.runAlg(basealg));
				g2 = g2.reduce(layer[i], reduce_method);

				string path = outdir;
				char buff[256];
				sprintf(buff, "layer%d_0.gen", i + 1);
				path = path + buff;
				layer[i].save(path);

				if (hicode_config["log_modularity"] == "true")
				{
					mods_ori_graph[0][i] = layer[i].calcModularity(g);
					mods_reduce_graph[0][i] = layer[i].calcModularity(g3);
				}

				for (size_t t = 0; t < truth.size(); ++t)
				{
					nmi_truth[t][0][i] = truth[t].calcNMI(layer[i]);
				}

				
			}

			layer_last = layer;
			layer.clear();

			//Stage 2 : Refinement
			for (int iterator = 0; iterator < iterator_times; ++iterator)
			{
				if (hicode_config["log_modularity"] == "true")
				{
					mods_ori_graph.push_back(vector<double>(layer_num));
					mods_reduce_graph.push_back(vector<double>(layer_num));
				}
				if (hicode_config["log_nmi_last"] == "true")
				{
					nmi_last.push_back(vector<double>(layer_num));
				}
				for (size_t t = 0; t < truth.size(); ++t)
				{
					nmi_truth[t].push_back(vector<double>(layer_num));
				}


				for (int i = 0; i < layer_num; ++i)
				{
					g2 = g;

					for (int j = 0; j < layer_num; ++j)
					{
						if (i == j)
							continue;
						g2 = g2.reduce(layer_last[j], reduce_method);
					}
					layer.push_back(g2.runAlg(basealg));

					string path = outdir;
					char buff[256];
					sprintf(buff, "layer%d_%d.gen", i + 1, iterator+1);
					path = path + buff;
					layer[i].save(path);

					if (hicode_config["log_modularity"] == "true")
					{
						mods_ori_graph[iterator+1][i] = layer[i].calcModularity(g);
						mods_reduce_graph[iterator+1][i] = layer[i].calcModularity(g2);
					}
					if (hicode_config["log_nmi_last"] == "true")
					{
						nmi_last[iterator][i] = layer[i].calcNMI(layer_last[i]);


					}
					for (size_t t = 0; t < truth.size(); ++t)
					{
						nmi_truth[t][iterator+1][i] = truth[t].calcNMI(layer[i]);
					}
				}
				bool not_change = true;
				for (int i = 0; i < layer_num; ++i)
				{
					if (nmi_last[iterator][i] != 1.0)
					{
						not_change = false;
						break;
					}
						
				}

				if (not_change)
					break;


				layer_last = layer;
				layer.clear();
			}


			//save csv
			if (hicode_config["log_modularity"] == "true")
			{
				Csv2rec csv(layer_num, mods_ori_graph);
				csv.save(outdir + "mods_ori_graph.txt");
				csv.setData(mods_reduce_graph);
				csv.save(outdir + "mods_reduce_graph.txt");
			}
			if (hicode_config["log_nmi_last"] == "true")
			{
				Csv2rec csv(layer_num, nmi_last);
				csv.save(outdir + "nmi_last.txt");
			}
			for (size_t t = 0; t < truth.size(); ++t)
			{
				Csv2rec csv(layer_num, nmi_truth[t]);
				csv.save(outdir + struth[t] + ".txt");
			}



			os::moveDir(outdir, graph_path);


		}



	}


	

	

	printf("------------\ndone\n");
	return 0;

}