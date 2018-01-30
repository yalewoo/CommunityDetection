#include "Graph.h"

#include <cstdlib>
#include <iostream>
#include <algorithm>

#include <fstream>
#include <sstream>
#include <string>

#include <cctype>

using std::fstream;
using std::string;
using std::stringstream;
using std::cout;
using std::endl;

using std::unique;
using std::swap;

#include "save.h"
#include "ylog.h"

double Graph::getSumWeighted() const
{
	double res = 0;
	for (size_t i = 0; i < edges.size(); ++i)
	{
		res += edges[i].w;
	}
	return res;
}

void Graph::addEdge(int x, int y, double w)
{
	min_node_id = min(min_node_id, x);
	max_node_id = max(max_node_id, y);
	edges.push_back(Edge(x, y, w));
}

bool Graph::load(char const *graph_path)
{
	clear();
	//先读一下图文件 看是否第一行是结点个数 是有权还是无权
	bool first_line_is_node_num = false;
	ifstream f(graph_path, std::ios::in);
	if (!f) return false;
	string s;
	getline(f, s);
	stringstream stream;
	stream.str(s);
	int id;
	int c = 0;
	while (stream >> id)
	{
		++c;
	}
	if (c == 1)
		first_line_is_node_num = true;

	getline(f, s);
	stream.clear();
	stream.str(s);
	c = 0;
	while (stream >> id)
	{
		++c;
	}
	if (c == 3)
		Weighted = true;
	f.close();


	FILE *fp = fopen(graph_path, "r");
	if (!fp)
		return false;

	//若第一行是结点个数 跳过这一行
	if (first_line_is_node_num)
	{
		char buff[256];
		fgets(buff, 256, fp);
	}
		
	
	if (Weighted)
		loadWeightedGraph(fp);
	else
		loadUnweightedGraph(fp);

	fclose(fp);

	//边(x,y)按照x升序，x相同是按照y升序
	sort(edges.begin(), edges.end());
	edges.erase(unique(edges.begin(), edges.end()), edges.end());

}

bool Graph::save(char const * graph_path)
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

//保存为无权图
bool Graph::saveUnweighted(char * graph_path)
{
	FILE *fp = fopen(graph_path, "w");
	if (!fp)
		return false;

	for (size_t i = 0; i < edges.size(); ++i)
	{
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

string int2str(int i)
{
	char s[100];
	sprintf(s, "%d", i);
	return s;
}
int linux_rand()
{
	static FILE * fp = fopen("F:/Project/CommunityDetection/tools/linux_rand_array.txt", "r");
	int n;
	if (fscanf(fp, "%d", &n) == 1)
		return n;
	else
		return 1;
}

Communities Graph::runInfomap(char * args)
{
	string Infomap = "\"" + config["Infomap_dir"] + "Infomap\"";
	save("tmp.graph");
	cmd(Infomap + " tmp.graph . -i link-list -s "+ int2str(linux_rand()));

	Communities cs;
	cs.setMaxNodeid(max_node_id);
	cs.loadInfomap0135("tmp.tree");
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


	saveUnweighted("tmp.graph");
	cmd(calcJaccards + " tmp.graph net.jaccs");
	cmd(clusterJaccards + " tmp.graph net.jaccs net.clusters net.mc_nc 0.1");

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

Communities Graph::runMod(char * args, int layer)
{
	ylog("runMod begin");
	string convert = "\"" + config["Mod_dir"] + "convert\"";
	string community = "\"" + config["Mod_dir"] + "community\"";
	

	save("tmp.graph");

	if (Weighted)
	{
		cmd(convert + " -i tmp.graph -o graph.bin -w graph.weights");
		cmd(community + " graph.bin  -l -1 -w graph.weights > mod.result");
	}
	else
	{
		cmd(convert + " -i tmp.graph -o graph.bin");
		cmd(community + " graph.bin  -l -1 -v > mod.result");
	}

	
	Communities cs;
	cs.setMaxNodeid(max_node_id);
	cs.loadMod("mod.result");

	ylog("runMod end");
	return cs;
	


}

Communities Graph::runAlg(string algname)
{
    //transform(algname.begin(), algname.end(), algname.begin(), tolower);

	if (algname == "mod")
		return runMod();
	if (algname == "infomap")
		return runInfomap();
	if (algname == "lc")
		return runLinkComm();
	if (algname == "oslom")
		return runOSLOM2();
	if (algname == "gce")
		return runGCE();
	if (algname == "demon")
		return runDemon();
	if (algname == "cfinder")
		return runCFinder();
	

	return Communities();
}

Graph Graph::remove(const Communities & cs)
{
	vector<vector<int> > cid = cs.getCommsOfEveryid(this->max_node_id);
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
	g.Weighted = Weighted;
	g.Directed = Directed;
	return g;
}

string Graph::print(bool show_detail)
{
	string res;
	char buff[256];
	printf("-----------------\n");
	if (Weighted)
	{
		printf("Weighted Graph: ");
		sprintf(buff, "Weighted Graph: ");
		res += buff;
	}
	else
	{
		printf("UnWeighted Graph: ");
		sprintf(buff, "UnWeighted Graph: ");
		res += buff;
	}
		
	printf("%d nodes, %d edges:\n", getNodeNum(), edges.size());
	sprintf(buff, "%d nodes, %d edges:\n", getNodeNum(), edges.size());
	res += buff;

	if (show_detail)
	{
		if (Weighted)
		{
			for (size_t i = 0; i < edges.size(); ++i)
			{
				printf("%d %d %lf\n", edges[i].x, edges[i].y, edges[i].w);
				sprintf(buff, "%d %d %lf\n", edges[i].x, edges[i].y, edges[i].w);
				res += buff;
			}
		}
		else
		{
			for (size_t i = 0; i < edges.size(); ++i)
			{
				printf("%d %d\n", edges[i].x, edges[i].y);
				sprintf(buff, "%d %d\n", edges[i].x, edges[i].y);
				res += buff;
			}
		}
	}

	return res;	
	
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

double Graph::calcModularity(const Communities & cs)  const
{
	if (edges.size() == 0)
		return 0;

	//社团个数
	int nc = cs.size();	

	//社团内部的边数
	vector<double> comm_inter_edge_num = getCommInterEdgeNum(cs);
	//cout << "内部边数ok" << endl;
	//社团连出的边数
	vector<double> comm_out_edge_num = getCommOutEdgeNum(cs);
	//cout << "连出边数ok" << endl;
	//社团内部点的度数之和
	vector<double> comm_inter_nodes_degree = getCommInterNodesDegree(comm_inter_edge_num, comm_out_edge_num);
	//cout << "度数之和ok" << endl;
	//总边数
	double m = getSumWeighted();

	double Q = 0;
	for (size_t i = 0; i < nc; ++i)
	{
		Q += (comm_inter_edge_num[i] / m) - (comm_inter_nodes_degree[i] / (2 * m)) * (comm_inter_nodes_degree[i] / (2 * m));
	}
	saveVector(comm_inter_edge_num, "comm_inter_edge_num.txt");
	saveVector(comm_out_edge_num, "comm_out_edge_num.txt");


	return Q;
}

//返回每个结点的度
vector<int> Graph::getDegree() const
{
	vector<int> d(max_node_id + 1, 0);
	for (size_t i = 0; i < edges.size(); ++i)
	{
		++d[edges[i].x];
		++d[edges[i].y];
	}

	return d;
}

//社团内部的边数
vector<double> Graph::getCommInterEdgeNum(const Communities & cs) const
{
	vector<double> v(cs.size(), 0);

	vector<vector<int> > csid = cs.getCommsOfEveryid(this->max_node_id);

	for (size_t i = 0; i < edges.size(); ++i)
	{
		int x = edges[i].x;
		int y = edges[i].y;
		double w = edges[i].w;

		

		vector<int> cap = Communities::intersection(csid[x], csid[y]);
		double cx = csid[x].size();	//cx个社团包含结点x
		double cy = csid[y].size();	//cy个社团包含结点y
		for (size_t j = 0; j < cap.size(); ++j)
		{
			v[cap[j]] += 0.5 * (1/cx + 1/cy) * w;
		}
	}

	return v;
}

//社团内部结点的度数之和
vector<double> Graph::getCommInterNodesDegree(vector<double> &comm_inter_edge_num, vector<double> & comm_out_edge_num) const
{
	vector<double> v(comm_inter_edge_num.size(), 0);

	for (size_t i = 0; i < comm_inter_edge_num.size(); ++i)
	{
		v[i] = 2 * comm_inter_edge_num[i] + comm_out_edge_num[i];
	}

	return v;
}
//社团连出去的边数
vector<double> Graph::getCommOutEdgeNum(const Communities & cs) const
{
	vector<double> v(cs.size(), 0);
	vector<vector<int> > csid = cs.getCommsOfEveryid(this->max_node_id);

	for (size_t i = 0; i < edges.size(); ++i)
	{
		int x = edges[i].x;
		int y = edges[i].y;
		double w = edges[i].w;

		

		vector<int> cap = Communities::difference(csid[x], csid[y]);
		double cx = csid[x].size();	//cx个社团包含结点x
		double cy = csid[y].size();	//cy个社团包含结点y
		if (cx != 0)
		{
			if (cy == 0) cy = 1;	
			for (size_t j = 0; j < cap.size(); ++j)
			{
				v[cap[j]] += (1 / cx + 1 - 1 / cy) * w;
				//v[cap[j]] += 0.5*(1 / cx + 1) * w;
			}
		}

		swap(x, y);
		cap = Communities::difference(csid[x], csid[y]);
		cx = csid[x].size();	//cx个社团包含结点x
		cy = csid[y].size();	//cy个社团包含结点y
		if (cx == 0)
			continue;
		if (cy == 0) cy = 1;
		for (size_t j = 0; j < cap.size(); ++j)
		{
			v[cap[j]] += (1 / cx + 1 - 1 / cy) * w;
			//v[cap[j]] += 0.5*(1 / cx + 1) * w;
		}
	}

	return v;
}

Graph Graph::removeEdgeLessThan(double thres)
{
	Graph res;
	res.Weighted = this->Weighted;
	for (size_t i = 0; i < edges.size(); ++i)
	{
		if (edges[i].w >= thres)
		{
			res.addEdge(edges[i].x, edges[i].y, edges[i].w);
		}
	}
	return res;
}

Graph Graph::reduce(Communities & cs, string & method)
{
	string m = method;
	for (size_t i = 0; i < m.size(); ++i)
	{
		m[i] = tolower(m[i]);
	}

	if (m == "reduceweight")
		return reduceWeight(cs);
	if (m == "remove")
		return removeEdge(cs);
	return *this;
}

Graph Graph::reduceWeight(Communities & cs)
{


	vector<vector<int> > cid = cs.getCommsOfEveryid(max_node_id);

	vector<double> d(cs.size(),0);	//保存社团内边数+社团连出的边数

	vector<double> e(cs.size(), 0);	//保存社团内部边数

	int n = this->getNodeNum();

	Graph g;
	g.Weighted = Weighted;
	g.Directed = Directed;

	for (size_t i_edges = 0; i_edges < edges.size(); ++i_edges)
	{
		int x = edges[i_edges].x;
		int y = edges[i_edges].y;
		double w = edges[i_edges].w;

		vector<int> cxs = cid[x];
		vector<int> cys = cid[y];

		//社团内部边加了两次，一个节点在社团里的边加了1次
		for (size_t i = 0; i < cxs.size(); ++i)
		{
			int cx = cxs[i];	//x属于社团cx
			d[cx] += w;
		}
		for (size_t j = 0; j < cys.size(); ++j)
		{
			int cy = cys[j];	//y属于社团cy
			d[cy] += w;
		}


		bool not_in_community_edge = true;

		//e 社团内部的边数 每个边加一次
		for (size_t i = 0; i < cxs.size(); ++i)
		{
			int cx = cxs[i];	//x属于社团cx
			for (size_t j = 0; j < cys.size(); ++j)
			{
				int cy = cys[i];
				if (cx == cy)
				{
					not_in_community_edge = false;
					e[cx] += w;
				}
			}
		}

		//不是同一个社团内部的边不受影响
		if (not_in_community_edge)
		{
			g.addEdge(x, y, w);
		}

	}


	//处理社团内部的边 每个边只会reduce一次
	for (size_t i_edges = 0; i_edges < edges.size(); ++i_edges)
	{
		int x = edges[i_edges].x;
		int y = edges[i_edges].y;
		double w = edges[i_edges].w;

		vector<int> cxs = cid[x];
		vector<int> cys = cid[y];

		for (size_t i = 0; i < cxs.size(); ++i)
		{
			bool reduced = false;
			int cx = cxs[i];	//x属于社团cx
			for (size_t j = 0; j < cys.size(); ++j)
			{
				int cy = cys[j];
				
				if (cx == cy)
				{
					int k = cx;
					int nk = cs.comms[k].size();
					double pk = e[k] / (0.5 * nk * (nk));
					//double pk = e[k] / (0.5 * nk * (nk - 1));
					//double qk2 = (d[k] - 2 * e[k]) / (nk * (0 - nk));
					//??? next line works bad
					double qk = (d[k] - 2 * e[k]) / (nk * (n - nk));
					//TODO qk可能会小于0？？
					if (pk > 0 && qk > 0)
					{
						double new_w = w*qk / pk;
						g.addEdge(x, y, new_w);
						if (new_w > 1 || new_w < -1)
							cout << new_w << endl;
					}
						
					//if (qk < 0)
						//cout << "qk<0" << endl;
					//cout << qk / pk << endl;

					reduced = true;
					break;
				}
			}
			if (reduced)
				break;
		}

	}


	g.save("F:/9157/9157IM2/Iteration/0/Layer2infunc.graph");
	return g;
}

Graph Graph::removeEdge(Communities & cs)
{
	return remove(cs);
}

Graph Graph::getSubGraph(set<int>& nodes)
{
	Graph res;
	res.Weighted = this->Weighted;

	set<int> & subnodes = nodes;


	for (size_t i = 0; i < edges.size(); ++i)
	{
		if (subnodes.find(edges[i].x) != subnodes.end() && subnodes.find(edges[i].y) != subnodes.end())
		{
			res.addEdge(edges[i].x, edges[i].y, edges[i].w);
		}
	}
	return res;
}

Graph Graph::getSubGraph(vector<int>& nodes)
{
	ylog("getSubGraph begin");
	Graph res;
	res.Weighted = this->Weighted;

	set<int> subnodes;
	for (size_t i = 0; i < nodes.size(); ++i)
	{
		subnodes.insert(nodes[i]);
	}

	for (size_t i = 0; i < edges.size(); ++i)
	{
		if (subnodes.find(edges[i].x) != subnodes.end() && subnodes.find(edges[i].y) != subnodes.end())
		{
			res.addEdge(edges[i].x, edges[i].y, edges[i].w);
		}
	}
	ylog("getSubGraph end");
	return res;
}


void Graph::loadWeightedGraph(FILE * fp)
{
	int x, y;
	double w;
	int count = 0;
	while (fscanf(fp, "%d%d%lf", &x, &y, &w) != -1)
	{	
		if (Directed)
		{
			if (x != y)
				addEdge(x, y, w);
		}
		else
		{
			if (x != y)
			{
				if (x > y)
					swap(x, y);
				addEdge(x, y, w);
			}
				
		}
		++count;
		//if (count % (2 << 10) == 0)
			//std::cout << count << std::endl;
		
	}
}

void Graph::loadUnweightedGraph(FILE * fp)
{
	int x, y;
	while (fscanf(fp, "%d%d", &x, &y) != -1)
	{
		if (Directed)
		{
			if (x != y)
				addEdge(x, y);
		}
		else
		{
			if (x != y)
			{
				if (x > y)
					swap(x, y);
				addEdge(x, y);
			}
				
			
		}
		
	}
}

void Graph::cmd(string s)
{
	//s += " >> log.txt";
	cmd(s.c_str());
}

void Graph::cmd(const char * s)
{
	if (SHOW_CMD)
		printf("Call: %s\n", s);
	system(s);
}

int Graph::getNodeNum()
{
	set<int> s;
	for (size_t i = 0; i < edges.size(); ++i)
	{
		s.insert(edges[i].x);
		s.insert(edges[i].y);
	}

	return s.size();
	//return 0;
}
