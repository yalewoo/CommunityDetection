#include "Communities.h"

#include <cctype>
#include "Communities.h"

#include <fstream>
#include <string>
#include <sstream>
#include <iostream>

#include "Graph.h"

using std::ifstream;
using std::string;
using std::stringstream;
using std::cout;
using std::endl;
using std::set_intersection;


void Communities::load(const char * fn)
{
	clear();

	ifstream f(fn, std::ios::in);


	while (!f.eof())
	{
		string s;
		getline(f, s);


		stringstream stream;
		stream << s;
		int id;
		Community c;
		while (stream >> id)
		{
			c.add(id);
		}
		c.sort();
		if (c.nodes.size() > 0)
			addCommunity(c);
	}
	removeSmallComm(1);
}

void Communities::loadInfomap(const char * fn)
{
	clear();
	vector<int> cid(max_node_id + 1);

	FILE *fp = fopen(fn, "r");
	if (!fp)
		return;
	char buff[512];
	fgets(buff, 512, fp);
	fgets(buff, 512, fp);

	int n;
	int lastn = 0;
	int node;

	Community c;
	while (fgets(buff, 512, fp))
	{
		//定位到倒数第二个数字 三层时1:2:3 会读出2
		int i = 0;
		while (buff[i] != ' ')
			++i;
		while (buff[i] != ':')
			--i;
		--i;
		while (isdigit(buff[i]))
		{
			--i;
			if (i == -1)
				break;
		}

		++i;
		//社团编号
		sscanf(buff + i, "%d", &n);

		i = strlen(buff);
		while (buff[i] != ' ')
			--i;
		++i;
		//节点号
		sscanf(buff + i, "%d", &node);

		
		cid[node] = n;
	}
	getCommsByCid(cid);
	removeSmallComm(1);
}

void Communities::loadLinkComm(const char * fn)
{
	clear();

	ifstream f(fn, std::ios::in);


	while (!f.eof())
	{
		string s;
		getline(f, s);

		for (size_t i = 0; i < s.size(); ++i)
		{
			if (s[i] == ',') s[i] = ' ';
		}
		stringstream stream;
		stream << s;
		int id;
		Community c;
		while (stream >> id)
		{
			c.add(id);
		}
		c.sort();
		if (c.nodes.size() > 0)
			addCommunity(c);

	}
	removeSmallComm(1);
}

void Communities::loadOSLOM2(const char * fn)
{
	clear();

	ifstream f(fn, std::ios::in);


	while (!f.eof())
	{
		string s;
		getline(f, s);

		if (s.size() > 0 && s[0] == '#')
			continue;

		stringstream stream;
		stream << s;
		int id;
		Community c;
		while (stream >> id)
		{
			c.add(id);
		}
		c.sort();
		if (c.nodes.size() > 0)
			addCommunity(c);
	}
	removeSmallComm(1);
}

void Communities::loadGCE(const char * fn)
{
	load(fn);
}

void Communities::loadDemon(const char * fn)
{
	clear();

	ifstream f(fn, std::ios::in);


	while (!f.eof())
	{
		string s;
		getline(f, s);

		for (size_t i = 0; i < s.size(); ++i)
		{
			if (s[i] == ',') s[i] = ' ';
		}


		stringstream stream;
		stream << s;
		int id;
		stream >> id;	//第一个数字是社团号舍弃
		Community c;
		while (stream >> id)
		{
			c.add(id);
		}
		c.sort();
		if (c.nodes.size() > 0)
			addCommunity(c);
	}
	removeSmallComm(1);
}

void Communities::loadCFinder(const char * fn)
{
	clear();

	ifstream f(fn, std::ios::in);


	while (!f.eof())
	{
		string s;
		getline(f, s);

		if (s.size() > 0 && s[0] == '#')
			continue;

		for (size_t i = 0; i < s.size(); ++i)
		{
			if (s[i] == ':') s[i] = ' ';
		}


		stringstream stream;
		stream << s;
		int id;
		stream >> id;	//第一个数字是社团号舍弃
		Community c;
		while (stream >> id)
		{
			c.add(id);
		}
		c.sort();
		if (c.nodes.size() > 0)
			addCommunity(c);
	}
	removeSmallComm(1);
}

void Communities::loadMod(const char * fn)
{
	clear();
	vector<int> cid(max_node_id + 1);

	ifstream f(fn, std::ios::in);


	string s;
	getline(f, s);

	stringstream stream;
	stream << s;
	int node_id;
	int comm_id;
	stream >> node_id >> comm_id;
	int last;
	Community c;
	c.clear();
	while (!f.eof())
	{
		cid[node_id] = comm_id;

		last = comm_id;
		c.add(node_id);

		getline(f, s);
		stringstream stream2;
		stream2 << s;
		stream2 >> node_id >> comm_id;
		
		if (node_id == 0)
			break;
		
	}
	getCommsByCid(cid);
	removeSmallComm(1);
}

vector<vector<int> > Communities::getCommsOfEveryid() const
{
	vector<vector<int> > res;
	res.resize(max_node_id+1);
	for (size_t i = 0; i < comms.size(); ++i)
	{
		const vector<int> & nodes = comms[i].nodes;
		for (size_t j = 0; j < nodes.size(); ++j)
		{
			res[nodes[j]].push_back(i);
		}
	}

	return res;
}

vector<int> Communities::intersection(vector<int>& a, vector<int>& b)
{
	sort(a.begin(), a.end());
	sort(b.begin(), b.end());

	vector<int> res(max(a.size(), b.size()));
	auto iter = set_intersection(a.begin(), a.end(), b.begin(), b.end(), res.begin());
	res.resize(iter - res.begin());

	return res;
}

double Communities::calcModularity(const Graph & g)
{
	int nc = size();
	vector<int> comm_inter_edge_num = g.getCommInterEdgeNum(*this);
	vector<int> comm_inter_nodes_degree = g.getCommInterNodesDegree(*this);
	double m = g.edges.size();	//总边数

	double Q = 0;
	for (size_t i = 0; i < nc; ++i)
	{
		Q += (comm_inter_edge_num[i] / m) - (comm_inter_nodes_degree[i] / (2 * m)) * (comm_inter_nodes_degree[i] / (2 * m));
	}

	this->Q = Q;
	return Q;
}

void Communities::getCommsByCid(const vector<int> &cid)
{
	int max_comm_id = *std::max_element(cid.begin(), cid.end());
	comms.resize(max_comm_id + 1);
	for (int i = 0; i < cid.size(); ++i)
	{
		comms[cid[i]].add(i);
	}
	removeSmallComm(1);
}

void Communities::removeSmallComm(int size)
{
	for (auto it = comms.begin(); it != comms.end();)
	{
		if (it->size() <= size)
			it = comms.erase(it);    //删除元素，返回值指向已删除元素的下一个位置    
		else
			++it;    //指向下一个位置
	}

}

void Communities::print()
{
	printf("-------------------------\n");
	printf("Modularity: %lf\n", Q);
	printf("%d communities:\n", comms.size());
	for (size_t i = 0; i < comms.size(); ++i)
	{
		for (size_t j = 0; j < comms[i].nodes.size(); ++j)
		{
			printf("%d ", comms[i].nodes[j]);
		}
		printf("\n");
	}
}

bool Communities::save(const char * fn)
{
	FILE * fp = fopen(fn, "w");
	if (!fp) return false;
	for (size_t i = 0; i < comms.size(); ++i)
	{
		for (size_t j = 0; j < comms[i].nodes.size(); ++j)
		{
			fprintf(fp, "%d ", comms[i].nodes[j]);
		}
		fprintf(fp, "\n");
	}
	fclose(fp);
	return true;
}

