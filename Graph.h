#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <string>
#include <map>
using std::vector;
using std::string;
using std::map;

#define SHOW_CMD false
#define LINE_BUFF_SIZE 1024

#include "Communities.h"

struct Edge {
	int x;
	int y;
	double w;

	Edge(int x, int y, double w = 1) : x(x), y(y), w(w) {}

	bool operator<(Edge & e) {
		if (x == e.x)
			return y < e.y;
		return x < e.x;
	}
	bool operator==(Edge & e) {
		return x == e.x && y == e.y;
	}
};
class Graph {
	vector<Edge> edges;
	double getSumWeighted() const;
	int max_node_id = -1;
	void addEdge(int x, int y, double w = 1);
	bool Weighted;
	bool Directed;

	static map<string, string> config;

	void loadWeightedGraph(FILE * fp);
	void loadUnweightedGraph(FILE * fp);

	void cmd(string s);
	void cmd(const char * s);

public:
	bool load(char * graph_path);
	bool save(char *graph_path);
	static bool loadConfig(char * config_path);
	
	Graph(bool weighted = false, bool directed = false) : Weighted(weighted), Directed(directed) {}

	Communities runInfomap(char * args = 0);
	Communities runLinkComm(char * args = 0);
	Communities runOSLOM2(char * args = 0);
	Communities runGCE(char * args = 0);
	Communities runDemon(char * args = 0);
	Communities runCFinder(char * args = 0);
	Communities runMod(char * args = 0);

	Graph remove(const Communities & cs);

	void print();
	bool create_dot_file(char *fn);
	void showPic(void);

	double calcModularity(const Communities & cs) const;	//计算模块度
	vector<int> getDegree() const;	//返回每个结点的度
	vector<double> getCommInterEdgeNum(const Communities & cs) const;	//返回每个社团内部的边数
	vector<double> getCommInterNodesDegree(vector<double> &comm_inter_edge_num, vector<double> & comm_out_edge_num) const;	//返回社团内部点的度数之和
	vector<double> getCommOutEdgeNum(const Communities & cs) const;	//返回社团连接其他社团的边数

	friend class Communities;
};




#endif