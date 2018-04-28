#include <cstdio>
#include <iostream>
#include "../Graph.h"
#include "../Config.h"
#include "../CsvOut.h"
#include "../os.h"

#include <numeric>

using std::cout;
using std::endl;
using std::accumulate;

extern Config hicode_config;




int main_hicode(int argc, char *argv[])
{
	
	string graph_path;
	if (argc >= 2)
		graph_path = argv[1];
	else
	{
		//graph_path = "Z:/synL2/";
		//graph_path = "F:/Local/Theme/Q_nips/0.001/";
		graph_path = "F:/HICODE_SUB/0426new/";
	}
		

	Graph g;
	g.load(graph_path + "graph");
	g.setWeighted(true);

	

	
	hicode_config.updateConfig(graph_path + "hicode_default.config");


	

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

	//输出baseline算法自身结果
	if (hicode_config["output_baseline_results"] == "true")
	{
		string outpath = "baseline/";
		os::mkdir(outpath);
		for (int i = 0; i < vbasealg.size(); ++i)
		{
			Communities cs;
			cs = g.runAlg(vbasealg[i]);
			cs.save(outpath + vbasealg[i] + ".gen");
		}
		os::moveDir(outpath, graph_path);
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

				string path;
				char buff[256];
				sprintf(buff, "layer%d_0.gen", i + 1);
				path = outdir + buff;
				layer[i].save(path);

				if (hicode_config["log_modularity"] == "true")
				{
					mods_ori_graph[0][i] = layer[i].calcModularity(g);
					mods_reduce_graph[0][i] = layer[i].calcModularity(g3);
				}

				//保存消边后的图
				if (hicode_config["log_save_reduce_graph"] == "true")
				{
					sprintf(buff, "graph%d_0.graph", i + 1);
					path = outdir + buff;
					g3.save(path);
				}

				for (size_t t = 0; t < truth.size(); ++t)
				{
					nmi_truth[t][0][i] = truth[t].calcNMI(layer[i]);
				}

				
			}

			//保存使mod最大的迭代结果
			int mod_origin_graph_iterator = 0, mod_layer_graph_iterator = 0;
			double mod_origin_graph, mod_layer_graph;
			vector<Communities> layer_mod_origin_graph;
			vector<Communities> layer_mod_layer_graph;
			if (hicode_config["log_modularity"] == "true")
			{
				mod_origin_graph = accumulate(mods_ori_graph[0].begin(), mods_ori_graph[0].end(), 0.0);
				mod_layer_graph = accumulate(mods_reduce_graph[0].begin(), mods_reduce_graph[0].end(), 0.0);
				layer_mod_origin_graph = layer;
				layer_mod_layer_graph = layer;
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
						if (j < layer.size())
							g2 = g2.reduce(layer[j], reduce_method);
						else
							g2 = g2.reduce(layer_last[j], reduce_method);
					}
					layer.push_back(g2.runAlg(basealg));

					//保存消边后的图
					if (hicode_config["log_save_reduce_graph"] == "true")
					{
						char buff[256];
						sprintf(buff, "graph%d_%d.graph", i + 1, iterator+1);
						string path = outdir + buff;
						g2.save(path);
					}


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

				//保存maxOrigin和maxLayer
				if (hicode_config["log_modularity"] == "true")
				{
					double new_mod_origin_graph = accumulate(mods_ori_graph[iterator + 1].begin(), mods_ori_graph[iterator + 1].end(), 0.0);
					double new_mod_layer_graph = accumulate(mods_reduce_graph[iterator + 1].begin(), mods_reduce_graph[iterator + 1].end(), 0.0);
					if (new_mod_origin_graph > mod_origin_graph)
					{
						mod_origin_graph = new_mod_origin_graph;
						layer_mod_origin_graph = layer;
						mod_origin_graph_iterator = iterator+1;
					}
					if (new_mod_layer_graph > mod_layer_graph)
					{
						mod_layer_graph = new_mod_layer_graph;
						layer_mod_layer_graph = layer;
						mod_layer_graph_iterator = iterator + 1;
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
				csv.savePNG(outdir + "mods_ori_graph.txt", outdir + "mods_ori_graph.png");
				csv.setData(mods_reduce_graph);
				csv.save(outdir + "mods_reduce_graph.txt");
				csv.savePNG(outdir + "mods_reduce_graph.txt", outdir + "mods_reduce_graph.png");
			}
			if (hicode_config["log_nmi_last"] == "true")
			{
				Csv2rec csv(layer_num, nmi_last);
				csv.save(outdir + "nmi_last.txt");
				csv.savePNG(outdir + "nmi_last.txt", outdir + "nmi_last.png");
			}
			for (size_t t = 0; t < truth.size(); ++t)
			{
				Csv2rec csv(layer_num, nmi_truth[t]);
				csv.save(outdir + struth[t] + ".txt");
				csv.savePNG(outdir + struth[t] + ".txt", outdir + struth[t] + ".png");
			}


			//保存maxOrigin和maxLayer
			if (hicode_config["log_modularity"] == "true")
			{
				for (int i = 0; i < layer_num; ++i)
				{
					string path;
					char buff[256];
					sprintf(buff, "maxOriginlayer%d.gen", i + 1);
					path = outdir + buff;
					layer_mod_origin_graph[i].save(path);

					sprintf(buff, "maxLayerlayer%d.gen", i + 1);
					path = outdir + buff;
					layer_mod_layer_graph[i].save(path);
				}

				FILE * fp = fopen((outdir + "maxmod.txt").c_str(), "w");
				fprintf(fp, "maxOrigin: %d\n", mod_origin_graph_iterator);
				fprintf(fp, "maxLayer: %d\n", mod_layer_graph_iterator);
				fclose(fp);


				//递归抓子图找社团
				if (hicode_config["use_r_sub"] == "true")
				{
					
					string layer_outdir = "hicode_" + basealg + "_" + reduce_method + "_"
						+ nlayer + "layers_layer_sub" + "/";


					string s_sub_times = hicode_config["sub_times"];
					int sub_times = atoi(s_sub_times.c_str());
					int sub_nodes_number_thres = atoi(hicode_config["sub_nodes_number_thres"].c_str());

					if (hicode_config["sub_maxori"] == "true")
					{
						string outdir = "hicode_" + basealg + "_" + reduce_method + "_"
							+ nlayer + "layers_ori_sub" + "/";
						os::mkdir(outdir);

						vector<Communities> subl = layer_mod_origin_graph;
						vector<Communities> subl_new;

						for (int iter_i = 1; iter_i <= sub_times; ++iter_i)
						{
							subl_new.clear();
							for (int li = 0; li < layer_num; ++li)
							{
								Communities & layer1 = subl[li];
								Communities sub;
								for (size_t i = 0; i < layer1.size(); ++i)
								{
									
									if (layer1.comms[i].size() > sub_nodes_number_thres)
									{
										Graph subg = g.getSubGraph(layer1.comms[i]);
										Communities subcs = subg.runAlg(basealg);
										if (subcs.size() > 1)
											sub.addCommunities(subcs);
									}
									else
									{
										//Communities subcs;
										//subcs.addCommunity(layer1.comms[i]);
										//sub.addCommunities(subcs);
									}

								}
								char buff[256];
								sprintf(buff, (outdir + "sub%d_%d.gen").c_str(),li+1, iter_i);
								printf((outdir + "sub%d_%d.gen").c_str(), li+1,iter_i);
								sub.save(buff);
								subl_new.push_back(sub);
							}
							subl = subl_new;
						}

						os::moveDir(outdir, graph_path);
					}
					if (hicode_config["sub_maxlayer"] == "true")
					{
						string outdir = "hicode_" + basealg + "_" + reduce_method + "_"
							+ nlayer + "layers_layer_sub" + "/";
						os::mkdir(outdir);

						vector<Communities> subl = layer_mod_layer_graph;
						vector<Communities> subl_new;

						for (int iter_i = 1; iter_i <= sub_times; ++iter_i)
						{
							subl_new.clear();
							for (int li = 0; li < layer_num; ++li)
							{
								Communities & layer1 = subl[li];
								Communities sub;
								for (size_t i = 0; i < layer1.size(); ++i)
								{

									if (layer1.comms[i].size() > sub_nodes_number_thres)
									{
										Graph subg = g.getSubGraph(layer1.comms[i]);
										Communities subcs = subg.runAlg(basealg);
										if (subcs.size() > 1)
											sub.addCommunities(subcs);
									}
									else
									{
										//Communities subcs;
										//subcs.addCommunity(layer1.comms[i]);
										//sub.addCommunities(subcs);
									}

								}
								char buff[256];
								sprintf(buff, (outdir + "sub%d_%d.gen").c_str(), li+1, iter_i);
								printf((outdir + "sub%d_%d.gen").c_str(), li+1, iter_i);
								sub.save(buff);
								subl_new.push_back(sub);
							}
							subl = subl_new;
						}
						os::moveDir(outdir, graph_path);
					}
					
					

				}

			}



			os::moveDir(outdir, graph_path);


		}



	}


	

	

	printf("------------\ndone\n");
	return 0;

}