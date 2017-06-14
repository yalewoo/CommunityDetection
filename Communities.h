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
class Graph;


//一个社团 包含多个结点
struct Community
{
	int parent;	//等级结构中记录 在上一层中所属的社团编号

	//社团内的所有结点
	vector<int> nodes;
	
	//社团内结点编号范围
	int max_node_id = -1;
	int min_node_id = INT_MAX;

	//添加一个节点 不排序 更新范围
	void add(int n) {
		max_node_id = max(max_node_id, n);
		min_node_id = min(min_node_id, n);
		nodes.push_back(n);
	}
	//排序和去重
	void sort() {
		std::sort(nodes.begin(), nodes.end());
		nodes.erase(unique(nodes.begin(), nodes.end()), nodes.end());
	}
	void clear() 
	{ 
		nodes.clear();
		max_node_id = -1;
		min_node_id = INT_MAX;
	}

	//返回社团中结点个数
	int size() const { return nodes.size(); }

	//pair的两个值分别为 返回值，下标
	//在truth中，找和本社团匹配最好的，计算指标 返回指标值和 匹配最好的在truth中的下标
	pair<double, int> JaccardPrecision(Communities & truth, FILE * fp);
	pair<double, int> Precision(Communities & truth, FILE * fp);

	//两个社团大小比较，用于排序
	//结点较多的社团排名靠前 结点数相同时，第一个结点编号小的靠前
	bool operator<(Community & e) {
		//return nodes.size() > e.nodes.size();
		if (nodes.size() > e.nodes.size())
			return true;
		else if (nodes.size() < e.nodes.size())
			return false;
		else
			return nodes[0] < e.nodes[0];
	}

};


//一种划分，包含多个社团
class Communities
{
public:
	//所有的社团
	vector< Community > comms;

	//modularity值，在调用calcModularity时更新
	double Q = 0;

	//转换cid到comms
	//cid[i]表示结点i所在社团
	void getCommsByCid(vector< Community > & comms, const vector<int> &cid);

	//最小和最大结点编号
	int max_node_id = -1;
	int min_node_id = INT_MAX;

	//有多少层等级结构
	int layer = 0;

	//等级结构
	//layers[0]是根
	//layers[layers.size()-1]是叶子
	vector<vector<Community> > layers;

private:
	//计算时的临时变量
	int nmi_max_node;
	bool nmi_half_more_nodes_positive;

public:
	//设置最大节点编号
	void setMaxNodeid(int max_nodeid) { max_node_id = max_nodeid; }

	//清空
	void clear()
	{ 
		comms.clear();
		layers.clear(); 

		max_node_id = -1;
		min_node_id = INT_MAX;
		layer = 0;
		Q = 0;
	}

	//添加一个社团 不排序 更新编号范围
	void addCommunity(Community c)
	{
		min_node_id = min(min_node_id, c.min_node_id);
		max_node_id = max(max_node_id, c.max_node_id);
		comms.push_back(c);
	}

	//把另一种划分的所有社团加到后面 不排序 更新编号范围
	void addCommunities(Communities & cs)
	{
		for (size_t i = 0; i < cs.comms.size(); ++i)
		{
			addCommunity(cs.comms[i]);
		}
	}

	//去掉结点数小于size的社团
	void removeSmallComm(vector< Community > & comm, int size);	


	string print(bool show_detail = false,  bool show_nodes = false);
	bool save(const char * fn);
	bool save(string fn);
	int size() const { return comms.size(); }

	Communities merge(Communities & other);

	//不重复结点个数
	int nodesNum();
	//所有社团结点数之和
	int sizeOfCommsSum() const;

	void load(string fn) { load(fn.c_str()); }
	void load(const char * fn);
	void loadInfomap(const char * fn);
	void loadInfomapHelp();
	void loadLinkComm(const char * fn);
	void loadOSLOM2(const char * fn);
	void loadGCE(const char * fn);
	void loadDemon(const char * fn);
	void loadCFinder(const char * fn);
	void loadMod(const char * fn, int level = -1);
	void loadCid(vector< Community > & comms, const char * fn);

	vector<vector<int> > getCommsOfEveryid(int max_id = 0) const;

	static vector<int> intersection(vector<int> & a, vector<int> & b);
	static vector<int> difference(vector<int> & a, vector<int> & b);
	static vector<int> setunion(vector<int> & a, vector<int> & b);

	friend class Graph;

	double calcModularity(const Graph & g);	//计算模块度

	double calcNMI(Communities & cs, string outdir = "") ;


	double H(Community & X) const;
	double H(Communities & X);

	double H_Xi_joint_Yj(Community & c1, Community & c2);
	double H_Xi_given_Yj(Community & Xi, Community & Yj);
	double H_Xi_given_Y(Community & c, Communities & cs);
	double H_Xi_given_Y_norm(Community & c, Communities & cs);
	double H_X_given_Y_norm(Communities & X, Communities & Y);	// = min H_c_c

	double h(double w, double n) const;
	double h(double x) const;


	static pair<double, double> Jaccard(Communities & Detected, Communities & truth, vector<int> & v_index, vector<double> &v_value, FILE * fp);
	static pair<double, double> JaccardRecall(Communities & Detected, Communities & truth, vector<int> & v_index, vector<double> &v_value, string dir);
	static pair<double, double> JaccardPrecision(Communities & Detected, Communities & truth, vector<int> & v_index, vector<double> &v_value, string dir);
	static pair<double, double> JaccardF1Score(Communities & Detected, Communities & truth, string dir);

	static pair<double, double> PRGeneral(Communities & Detected, Communities & truth, vector<int> & v_index, vector<double> &v_value, FILE * fp);
	static pair<double, double> Recall(Communities & Detected, Communities & truth, vector<int> & v_index, vector<double> &v_value, string dir);
	static pair<double, double> Precision(Communities & Detected, Communities & truth, vector<int> & v_index, vector<double> &v_value, string dir);
	static pair<double, double> F1Score(Communities & Detected, Communities & truth, string dir);

	static bool mkdir(string dir);
};


#endif