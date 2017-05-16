#include "Graph.h"

#include <cstdlib>

#include <algorithm>


using std::unique;

void Graph::addEdge(int x, int y, double w)
{
	edges.push_back(Edge(x, y, w));
}

bool Graph::load(char * graph_path)
{
	FILE *fp = fopen(graph_path, "r");
	if (!fp)
		return false;
	
	if (Weighted)
		loadWeightedGraph(fp);
	else
		loadUnweightedGraph(fp);

	fclose(fp);

	sort(edges.begin(), edges.end());
	edges.erase(unique(edges.begin(), edges.end()), edges.end());

}

bool Graph::save(char * graph_path)
{
	FILE *fp = fopen(graph_path, "w");
	if (!fp)
		return false;

	for (size_t i = 0; i < edges.size(); ++i)
	{
		if (Weighted)
			fprintf(fp, "%d\t%d\t%lf\n", edges[i].x, edges[i].y, edges[i].w);
		else
			fprintf(fp, "%d\t%d\n", edges[i].x, edges[i].y);
	}
	fclose(fp);
}

bool Graph::loadConfig(char * config_path)
{
	FILE* fin = fopen(config_path, "r");
	if (!fin)	return false;
	config.clear();
	char buff[LINE_BUFF_SIZE];
	while (fgets(buff, LINE_BUFF_SIZE, fin))
	{
		if (strlen(buff) > 0 && buff[strlen(buff) - 1] == '\n')
			buff[strlen(buff) - 1] = '\0';
		string s(buff);

		if (s.compare(0, 1, "#") == 0)
			continue;
		size_t pos = s.find('=');
		string name = s.substr(0, pos);
		string value = s.substr(pos + 1);
		config[name] = value;
	}
		
	fclose(fin);


	return true;
}

Communities Graph::runInfomap(char * args)
{
	string Infomap = "\"" + config["Infomap_dir"] + "Infomap\"";
	save("tmp.graph");
	cmd(Infomap + " tmp.graph . -z -i link-list");

	Communities cs;
	cs.loadInfomap("tmp.tree");
	return cs;
}

Communities Graph::runLinkComm(char * args)
{
	//./calcJaccards net.pairs net.jaccs
	//. / clusterJaccards net.pairs net.jaccs net.clusters net.mc_nc THRESHOLD
	string calcJaccards = "\"" + config["LinkComm_dir"];
	string clusterJaccards = "\"" + config["LinkComm_dir"];


	calcJaccards += "calcJaccards\"";
	clusterJaccards += "clusterJaccards\"";


	save("tmp.graph");
	cmd(calcJaccards + " tmp.graph net.jaccs");
	cmd(clusterJaccards + " tmp.graph net.jaccs net.clusters net.mc_nc 0.2");

	Communities cs;
	cs.loadLinkComm("net.clusters");

	return cs;
}

Communities Graph::runOSLOM2(char * args)
{
	string oslom_undir = "\"" + config["OSLOM2_dir"];
	string oslom_dir = "\"" + config["OSLOM2_dir"];


	oslom_undir += "oslom_undir\"";
	oslom_dir += "oslom_dir\"";


	save("tmp.graph");
	if (Weighted)
		cmd(oslom_undir + " -f tmp.graph -w");
	else
		cmd(oslom_undir + " -f tmp.graph -uw");

	Communities cs;
	cs.loadOSLOM2("tmp.graph_oslo_files/tp");

	return cs;

}

Communities Graph::runGCE(char * args)
{
	string GCE = "\"" + config["GCE_dir"] + "GCECommunityFinder\"";
	save("tmp.graph");
	//cmd(GCE + " tmp.graph 3 0.6 1.0 0.75 > gce.log | tee gce.gen");
	cmd("\"" + GCE + " tmp.graph 3 0.6 1.0 0.75 2> gen.log | tee gce.gen\"");

	Communities cs;
	cs.loadGCE("gce.gen");
	return cs;
}

Communities Graph::runDemon(char * args)
{
	string python2 = config["python2_exe"];
	string demonlaunch = config["Demon_launch"];
	save("tmp.graph");
	//cmd(GCE + " tmp.graph 3 0.6 1.0 0.75 > gce.log | tee gce.gen");
	cmd(python2 + " " + demonlaunch + " tmp.graph");

	Communities cs;
	cs.loadDemon("communities");
	return cs;
}

Communities Graph::runCFinder(char * args)
{
	string Cfinder_exe = config["Cfinder_exe"];
	string Cfinder_licence = config["Cfinder_licence"];

	save("tmp.graph");

	cmd("rm -r CFinderOutput");
	cmd(Cfinder_exe + " -i tmp.graph -l " + Cfinder_licence + " -o CFinderOutput");

	Communities cs;
	cs.loadCFinder("CFinderOutput/k=3/communities");
	return cs;
}

Communities Graph::runMod(char * args)
{
	string convert = "\"" + config["Mod_dir"] + "convert\"";
	string community = "\"" + config["Mod_dir"] + "community\"";


	save("tmp.graph");

	if (Weighted)
	{
		cmd(convert + " -i tmp.graph -o graph.bin -w graph.weights");
		cmd(community + " graph.bin  -l -1 -w graph.weights > graph.tree");
	}
	else
	{
		cmd(convert + " -i tmp.graph -o graph.bin");
		cmd(community + " graph.bin  -l -1 -v > mod.result");
	}
	

	Communities cs;
	cs.loadMod("mod.result");
	return cs;

}

Graph Graph::remove(const Communities & cs)
{
	vector<vector<int> > cid = cs.getCommsOfEveryid();
	Graph g;
	for (size_t i = 0; i < edges.size(); ++i)
	{
		int x = edges[i].x;
		int y = edges[i].y;
		double w = edges[i].w;
		vector<int> insertofxy = Communities::intersection(cid[x], cid[y]);
		if (insertofxy.empty())
		{
			g.addEdge(x, y, w);
		}
	}

	return g;
}

void Graph::print()
{
	printf("-----------------\n");
	printf("%d edges:\n", edges.size());
	for (size_t i = 0; i < edges.size(); ++i)
	{
		printf("%d %d\n", edges[i].x, edges[i].y);
	}
}
bool Graph::create_dot_file(char *fn)
{
	FILE * fp = fopen(fn, "w");
	if (!fp)
		return false;

	if (Weighted)	//有权图
	{
		if (!Directed)	//无向图 有权
		{
			for (size_t i = 0; i < edges.size(); ++i)
			{
				
			}
		}
		else //有向图 有权
		{

		}
	}
	else
	{
		if (!Directed)	//无向图 无权图
		{
			fprintf(fp, "graph g {\n");

			for (size_t i = 0; i < edges.size(); ++i)
			{
				fprintf(fp, "\t%d -- %d;\n", edges[i].x, edges[i].y);
			}
			fprintf(fp, "}");
		}
		else //有向图 无权图
		{

		}

	}

	fclose(fp);
	return true;
}
void Graph::showPic(void)
{
	create_dot_file("tmp.dot");
	string dot = config["Graphviz_dot"];
	cmd(dot + " -T png tmp.dot -o graph.png");
	cmd("graph.png");
	printf("Press Enter to continue...\n");
	getchar();
}


void Graph::loadWeightedGraph(FILE * fp)
{
	int x, y;
	double w;
	while (fscanf(fp, "%d%d%lf", &x, &y, &w) != -1)
	{	
		if (x != y)
			edges.push_back(Edge(x, y, w));
	}
}

void Graph::loadUnweightedGraph(FILE * fp)
{
	int x, y;
	while (fscanf(fp, "%d%d", &x, &y) != -1)
	{
		if (x != y)
			edges.push_back(Edge(x, y));
	}
}

void Graph::cmd(string s)
{
	cmd(s.c_str());
}

void Graph::cmd(const char * s)
{
	printf("Call: %s\n", s);
	system(s);
}
