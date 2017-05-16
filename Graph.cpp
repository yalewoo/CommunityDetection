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
	cmd(Infomap + " graph.txt . -z -i link-list");

	Communities cs;
	cs.loadInfomap("graph.tree");
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
