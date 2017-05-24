#ifndef COMMUNITIES_H
#define COMMUNITIES_H

#include <vector>
#include <set>
#include <algorithm>
#include <utility>
#include <string>
using std::vector;
using std::unique;
using std::sort;
using std::max;
using std::min;
using std::set;
using std::pair;
using std::string;

class Communities;

struct Community
{
	vector<int> nodes;
	int max_node_id = -1;
	int min_node_id = INT_MAX;


	void add(int n) {
		max_node_id = max(max_node_id, n);
		min_node_id = min(min_node_id, n);
		nodes.push_back(n);
	}
	void sort() {
		std::sort(nodes.begin(), nodes.end());
		nodes.erase(unique(nodes.begin(), nodes.end()), nodes.end());
	}
	void clear() { nodes.clear(); }
	int size() const { return nodes.size(); }

	//返回值，下标
	pair<double, int> Precision(Communities & truth, FILE * fp);

};
class Communities
{
public:
	vector< Community > comms;
	double Q = 0;
	void getCommsByCid(const vector<int> &cid);	//转换cid到comms
	int max_node_id = -1;
	int min_node_id = INT_MAX;
	int nmi_max_node;
	bool nmi_half_more_nodes_positive;

public:
	void setMaxNodeid(int max_nodeid) { max_node_id = max_nodeid; }
	void clear() { comms.clear(); }

	void addCommunity(Community c)
	{
		min_node_id = min(min_node_id, c.min_node_id);
		max_node_id = max(max_node_id, c.max_node_id);
		comms.push_back(c);
	}
	void removeSmallComm(int size = 1);	//去掉结点数小于size的社团
	void print(bool show_nodes = false);
	bool save(const char * fn);
	bool save(string fn);
	int size() const { return comms.size(); }

	//所有社团结点数之和
	int sizeOfCommsSum() const;

	void load(const char * fn);
	void loadInfomap(const char * fn);
	void loadLinkComm(const char * fn);
	void loadOSLOM2(const char * fn);
	void loadGCE(const char * fn);
	void loadDemon(const char * fn);
	void loadCFinder(const char * fn);
	void loadMod(const char * fn);

	vector<vector<int> > getCommsOfEveryid(int max_id = 0) const;

	static vector<int> intersection(vector<int> & a, vector<int> & b);
	static vector<int> difference(vector<int> & a, vector<int> & b);
	static vector<int> setunion(vector<int> & a, vector<int> & b);

	friend class Graph;

	double calcModularity(const Graph & g);	//计算模块度

	double calcNMI(Communities & cs) ;


	double H(Community & X) const;
	double H(Communities & X);

	double H_Xi_joint_Yj(Community & c1, Community & c2);
	double H_Xi_given_Yj(Community & Xi, Community & Yj);
	double H_Xi_given_Y(Community & c, Communities & cs);
	double H_Xi_given_Y_norm(Community & c, Communities & cs);
	double H_X_given_Y_norm(Communities & X, Communities & Y);	// = min H_c_c

	double h(double w, double n) const;
	double h(double x) const;


	static double Jaccard(Communities & Detected, Communities & truth, vector<int> & v_index, vector<double> &v_value, FILE * fp);
	static double Recall(Communities & Detected, Communities & truth, vector<int> & v_index, vector<double> &v_value, string dir);
	static double Precision(Communities & Detected, Communities & truth, vector<int> & v_index, vector<double> &v_value, string dir);
	static double F1Score(Communities & Detected, Communities & truth, string dir);
	static bool mkdir(string dir);
};


#endif