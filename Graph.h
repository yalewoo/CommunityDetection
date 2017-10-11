#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <string>
#include <map>
using std::vector;
using std::string;
using std::map;

//为true时，显示调用的cmd命令行
#define SHOW_CMD true

#define LINE_BUFF_SIZE 1024

#include "Communities.h"

#ifdef _WIN32

#define ALG_CONFIG_PATH "F:/Project/CommunityDetection/config_win.txt"

#else

#define ALG_CONFIG_PATH "/home/dell/"

#endif


//图中的一条边
struct Edge {
	int x;
	int y;
	double w;

	Edge(int x, int y, double w = 1) : x(x), y(y), w(w) {}

	//边比较大小(x,y)先比较x，在比较y
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
	/******私有变量*******/

	//图中的所有边
	vector<Edge> edges;

	bool Weighted;	//带权图
	bool Directed;	//有向图

	//图中结点编号范围
	int max_node_id = -1;
	int min_node_id = INT_MAX;

	//记录社团检测算法的路径信息，需要在main函数中初始化并加载
	static map<string, string> config;

	/******私有方法*******/

	//添加一条边到最后 不排序 最大最小结点编号更新
	void addEdge(int x, int y, double w = 1);

	//返回所有边的权值之和
	double getSumWeighted() const;

	//加载带权图或无权图
	void loadWeightedGraph(FILE * fp);
	void loadUnweightedGraph(FILE * fp);

	//运行cmd命令
	static void cmd(string s);
	static void cmd(const char * s);

public:
	//清空边表和编号范围
	void clear() {
		edges.clear();
		max_node_id = -1;
		min_node_id = INT_MAX;
	}

	//返回图中不重复的结点数目
	int getNodeNum();

	//返回图中边的数目
	int getEdgeNum() { return edges.size(); }

	//设置带权图或无权图
	void setWeighted(bool weighted) { Weighted = weighted; }

	//从文件加载图
	//文件每行表示一条边 有权：x y w 无权： x y 
	//若每行3个数字 则当做有权图 
	//若每行2个数字 当做无权图
	//文件第一行可以是一个数字（例如结点个数）
	//无向图中 只有x<y的边会被保存
	//边表会进行排序和去重
	//若文件打开失败返回false
	bool load(string fn) { return load(fn.c_str()); }
	bool load(char const * graph_path);

	//保存图到文件 Weighted决定输出带权还是不带权
	bool save(string fn) 
	{ 
		return save(fn.c_str()); 
	}
	bool save(char const * graph_path);
	//按照不带权的方式保存
	bool saveUnweighted(char * graph_path);

	//读配置文件 读入社团检测算法(mod, infomap, oslom, etc.)的路径
	static bool loadConfig(char * config_path);
	
	//新建一个图 可选weighted和directed类型
	Graph(bool weighted = false, bool directed = false) : Weighted(weighted), Directed(directed) {}

	//在图上调用社团检测算法 参数目前都没有用到	
	//调用某个算法，algname的值需小写，可以是mod,infomap,oslom,gce,demon,cfinder
	Communities runAlg(string algname);
	//具体算法
	Communities runInfomap(char * args = 0);
	Communities runLinkComm(char * args = 0);
	Communities runOSLOM2(char * args = 0);
	Communities runGCE(char * args = 0);
	Communities runDemon(char * args = 0);
	Communities runCFinder(char * args = 0);
	Communities runMod(char * args = NULL , int layer = -1);


	

	//打印图的相关信息，show_detail时会输出每一条边
	//返回值保存了打印的输出
	string print(bool show_detail = false);

	//输出图对应的dot文件 用于Graphviz
	bool create_dot_file(char *fn);
	//用Graphviz显示图片
	void showPic(void);


	//计算模块度
	double calcModularity(const Communities & cs) const;
	vector<int> getDegree() const;	//返回每个结点的度
	vector<double> getCommInterEdgeNum(const Communities & cs) const;	//返回每个社团内部的边数
	vector<double> getCommInterNodesDegree(vector<double> &comm_inter_edge_num, vector<double> & comm_out_edge_num) const;	//返回社团内部点的度数之和
	vector<double> getCommOutEdgeNum(const Communities & cs) const;	//返回社团连接其他社团的边数

	friend class Communities;
	friend class Csv2rec;
	friend class MainWindow;	//Qt使用


	//下面对图的消边操作 均不影响原图，而是以返回值的形式返回

	//只保留权值大于等于thres的边
	Graph removeEdgeLessThan(double thres);
	//返回  去掉社团内部的所有边 之后的图
	Graph remove(const Communities & cs);
	
	//reduceWeight消边
	Graph reduce(Communities & cs, string & method);	//可选：reduceweight;remove
	Graph reduceWeight(Communities & cs);
	//remove消边
	Graph removeEdge(Communities & cs);

	//返回只有nodes节点的子图
	Graph getSubGraph(set<int> & nodes);
	Graph getSubGraph(vector<int> & nodes);
	Graph getSubGraph(Community & c) { return getSubGraph(c.nodes); }

};




#endif