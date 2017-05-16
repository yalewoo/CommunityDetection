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

	Community()
	{
		nodes.clear();
	}
	void add(int n)
	{
		max_node_id = max(max_node_id, n);
		nodes.push_back(n);
	}
	void sort()
	{
		std::sort(nodes.begin(), nodes.end());
		nodes.erase(unique(nodes.begin(), nodes.end()), nodes.end());
	}
	void clear()
	{
		nodes.clear();
	}
};
class Communities
{
	vector< Community > comms;
	int max_node_id = -1;
public:
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
	void print();
	bool save(const char * fn);
	int size() { return comms.size(); }


	void loadInfomap(const char * fn);
	void loadLinkComm(const char * fn);
	void loadOSLOM2(const char * fn);
	void loadGCE(const char * fn);
	void loadDemon(const char * fn);
	void loadCFinder(const char * fn);
	void loadMod(const char * fn);

	vector<vector<int> > getCommsOfEveryid() const;

	static vector<int> intersection(vector<int> & a, vector<int> & b);
	
};


#endif