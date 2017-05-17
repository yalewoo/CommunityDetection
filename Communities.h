#ifndef COMMUNITIES_H
#define COMMUNITIES_H

#include <vector>
#include <set>
#include <algorithm>
using std::vector;
using std::unique;
using std::sort;
using std::max;
using std::set;

struct Community
{
	vector<int> nodes;
	int max_node_id = -1;

	Community() { nodes.clear(); }
	void add(int n) {
		max_node_id = max(max_node_id, n);
		nodes.push_back(n);
	}
	void sort() {
		std::sort(nodes.begin(), nodes.end());
		nodes.erase(unique(nodes.begin(), nodes.end()), nodes.end());
	}
	void clear() { nodes.clear(); }
	int size() const { return nodes.size(); }
};
class Communities
{

	vector< Community > comms;
	double Q = 0;
	void getCommsByCid(const vector<int> &cid);	//转换cid到comms
	int max_node_id = -1;
public:
	void setMaxNodeid(int max_nodeid) { max_node_id = max_nodeid; }
	void clear() { comms.clear(); }
	Communities()
	{
		comms.clear();
	}
	void addCommunity(Community c)
	{
		max_node_id = max(max_node_id, c.max_node_id);
		comms.push_back(c);
	}
	void removeSmallComm(int size = 1);	//去掉结点数小于size的社团
	void print();
	bool save(const char * fn);
	int size() const { return comms.size(); }

	void load(const char * fn);
	void loadInfomap(const char * fn);
	void loadLinkComm(const char * fn);
	void loadOSLOM2(const char * fn);
	void loadGCE(const char * fn);
	void loadDemon(const char * fn);
	void loadCFinder(const char * fn);
	void loadMod(const char * fn);

	vector<vector<int> > getCommsOfEveryid() const;

	static vector<int> intersection(vector<int> & a, vector<int> & b);
	static vector<int> difference(vector<int> & a, vector<int> & b);

	friend class Graph;

	double calcModularity(const Graph & g);	//计算模块度
};


#endif