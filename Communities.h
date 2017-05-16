#ifndef COMMUNITIES_H
#define COMMUNITIES_H

#include <vector>
#include <algorithm>
using std::vector;
using std::unique;
using std::sort;

struct Community
{
	vector<int> nodes;

	Community()
	{
		nodes.clear();
	}
	void add(int n)
	{
		nodes.push_back(n);
	}
	void sort()
	{
		std::sort(nodes.begin(), nodes.end());
		nodes.erase(unique(nodes.begin(), nodes.end()), nodes.end());
	}
};
class Communities
{
	vector< Community > comms;

public:
	void clear() { comms.clear(); }
	Communities()
	{
		comms.clear();
	}
	void addCommunity(Community c)
	{
		comms.push_back(c);
	}
	void print();
	int size() { return comms.size(); }


	void loadInfomap(const char * fn);
	void loadLinkComm(const char * fn);
	void loadOSLOM2(const char * fn);
	void loadGCE(const char * fn);
	void loadDemon(const char * fn);
	void loadCFinder(const char * fn);

	
};


#endif