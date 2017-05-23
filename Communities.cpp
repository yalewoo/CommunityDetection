#include "Communities.h"

#include <cctype>
#include "Communities.h"

#include <fstream>
#include <string>
#include <sstream>
#include <iostream>

#include <cmath>
double log2(double x)
{
	return log(x) / log(2);
}

#include "Graph.h"

using std::ifstream;
using std::string;
using std::stringstream;
using std::cout;
using std::endl;
using std::min;
using std::make_pair;



int Communities::sizeOfCommsSum() const
{
	int res = 0;
	for (size_t i = 0; i < comms.size(); ++i)
	{
		res += comms[i].nodes.size();
	}
	return res;
}

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

vector<vector<int> > Communities::getCommsOfEveryid(int max_id) const
{
	vector<vector<int> > res;
	res.resize(max(max_id, max_node_id) +1);
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
	auto iter = std::set_intersection(a.begin(), a.end(), b.begin(), b.end(), res.begin());
	res.resize(iter - res.begin());

	return res;
}

vector<int> Communities::difference(vector<int>& a, vector<int>& b)
{
	sort(a.begin(), a.end());
	sort(b.begin(), b.end());

	vector<int> res(max(a.size(), b.size()));
	auto iter = std::set_difference(a.begin(), a.end(), b.begin(), b.end(), res.begin());
	res.resize(iter - res.begin());

	return res;
}

vector<int> Communities::setunion(vector<int>& a, vector<int>& b)
{
	sort(a.begin(), a.end());
	sort(b.begin(), b.end());

	vector<int> res(a.size() + b.size());
	auto iter = std::set_union(a.begin(), a.end(), b.begin(), b.end(), res.begin());
	res.resize(iter - res.begin());

	return res;
}

double Communities::calcModularity(const Graph & g)
{
	this->Q = g.calcModularity(*this);
	return Q;
}

//返回X和Y中不重复元素个数
int getNodesNum(Communities & cs, Communities & cs2)
{
	vector<Community> & X = cs.comms;
	vector<Community> & Y = cs2.comms;
	set<int> s;
	for (size_t i = 0; i < X.size(); ++i)
	{
		for (size_t j = 0; j < X[i].size(); ++j)
			s.insert(X[i].nodes[j]);
	}
	for (size_t i = 0; i < Y.size(); ++i)
	{
		for (size_t j = 0; j < Y[i].size(); ++j)
			s.insert(Y[i].nodes[j]);
	}
	return s.size();
}

double Communities::calcNMI(Communities & cs)
{
	double res = 0;
	Communities & X = *this;
	Communities & Y = cs;

	if (X.size() == 0 || Y.size() == 0)
		return 0;

	nmi_max_node = getNodesNum(*this, cs);
	//nmi_max_node = max(X.max_node_id, Y.max_node_id) - min(X.min_node_id, Y.min_node_id) + 1;
	//nmi_max_node = max(X.max_node_id, Y.max_node_id);
	
	double H_X_Y = H_X_given_Y_norm(X, Y);
	double H_Y_X = H_X_given_Y_norm(Y, X);
	//res = 1 - 0.5 * (H_X_Y + H_Y_X);
	double HX = H(X);
	double HY = H(Y);
	res = 1 - 0.5 * (H_X_Y + H_Y_X);
	return res;
}


/*
信息熵 H(X)，X是一个社团
例如： X = [2, 4, 5] 表示结点2,4,5在该社团中
若图中共有n=9个结点（编号0-8） 则X还可以看做一个表的形式
X = [0,0,1,0,1,1,0,0,0] X的取值有2种，即0和1
1出现的概率是p1 = (X.size()/n) = 3/9 , 0出现的概率是p0 = (n-X.size())/n = 6/9 
则 H(X) = h(p0)+h(p1)   //h(x)=-x*log2(x)
*/
double Communities::H(Community & X) const
{
	double n = nmi_max_node;
	double p1 = X.nodes.size() / n;
	double p0 = 1 - p1;
	return h(p0) + h(p1);
}



double Communities::H(Communities & X)
{
	double res = 0;
	for (size_t i = 0; i < X.comms.size(); ++i)
	{
		int px = X.comms[i].nodes.size();
		if (px > 0)
			res += H(X.comms[i]);
	}
	return res;
}

/*X和Y的联合熵H(Xi,Yj)
X和Y共有4种组合(X=1,Y=1)(X=0,Y=1)(X=1,Y=0)(X=0,Y=0)
P(X=1,Y=1) = |X ∩ Y|
P(X=0,Y=1) = |Y - X|
P(X=1,Y=0) = |X - Y|
P(X=0,Y=0) = n - |X ∪ Y|
H(X,Y) = h(P(X=1,Y=1)) + h(P(X=0,Y=1)) 
       + h(P(X=1,Y=0)) + h(P(X=0,Y=0))  //h(x)=-x*log2(x)
       
*/
double Communities::H_Xi_joint_Yj(Community & Xi, Community & Yj)
{
	vector<int> & X = Xi.nodes;
	vector<int> & Y = Yj.nodes;

	double n = nmi_max_node;

	vector<int> inter = intersection(X, Y);
	vector<int> X_Y = difference(X, Y);
	vector<int> Y_X = difference(Y, X);

	int n11 = inter.size(); //1 1
	int n01 = Y_X.size();	// 0 1
	int n10 = X_Y.size(); // 1 0
	int n00 = n - n11 - n01 - n10; // 0 0

	double p11 = n11 / n;
	double p01 = n01 / n;
	double p10 = n10 / n;
	double p00 = n00 / n;

	//如果这个条件没有满足 nmi_half_more_nodes_positive不会变为true
	if (h(p11) + h(p00) >= h(p01) + h(p10))
	{
		
	}
	else
	{
		//return -log(0);
		nmi_half_more_nodes_positive = false;
		return H(Xi);	//多加H(Yj)是因为在H_Xi_given_Yj中减去
	}

	return h(p11) + h(p01) + h(p10) + h(p00);
}

/*条件熵H(Xi|Yj) = H(Xi,Yj) - H(Yj)
*/
double Communities::H_Xi_given_Yj(Community & Xi, Community & Yj)
{
	nmi_half_more_nodes_positive = true;
	double HXY = H_Xi_joint_Yj(Xi, Yj);
	if (nmi_half_more_nodes_positive == false)
	{
		return HXY;
	}
	else
	{
		return HXY - H(Yj);
	}
}
double Communities::h(double w, double n) const
{
	return w > 0 ? -1 * (w/n) * log2(w / n) : 0;
}
double Communities::h(double x) const
{
	return x > 0 ? -1 * x * log2(x) : 0;
}

//v_index[i]保存和i最相思的truth编号
double Communities::Precision(Communities & Detected, Communities & truth, vector<int>& v_index, vector<double> &v_value)
{
	v_index.clear();
	v_value.clear();
	v_index.resize(Detected.size());
	v_value.resize(Detected.size());
	double m = Detected.sizeOfCommsSum();
	double res = 0;
	for (size_t i = 0; i < Detected.size(); ++i)
	{
		pair<double, int> p = Detected.comms[i].Precision(truth);
		v_value[i] = p.first;
		res += p.first * Detected.comms[i].size() / m;
		v_index[i] = p.second;
	}

	return res;
}

double Communities::Recall(Communities & Detected, Communities & truth, vector<int>& v_index, vector<double> &v_value)
{
	return Precision(truth, Detected, v_index, v_value);
}

double Communities::F1Score(Communities & Detected, Communities & truth)
{
	vector<int> tmp;
	vector<double> tmpd;
	double P = Precision(Detected, truth, tmp, tmpd);
	double R = Recall(Detected, truth, tmp, tmpd);
	return 2 * P * R / (P + R);
}

/*H(Xi|Y) = min { H(Xi|Yj) ,for all j }
if [ h(p11) + h(p00) > h(p01) + h(p10) ] never occur, H(Xi|Y) = H(Xi)
*/
double Communities::H_Xi_given_Y(Community & Xi, Communities & Y)
{
	double res = H_Xi_given_Yj(Xi, Y.comms[0]);
	for (size_t i = 1; i < Y.size(); ++i)
	{
		res = std::min(res, H_Xi_given_Yj(Xi, Y.comms[i]));
	}


	return res;
}
/*H(Xi|Y)_norm = H(Xi|Y) / H(Xi)
*/
double Communities::H_Xi_given_Y_norm(Community & Xi, Communities & Y)
{
	return H_Xi_given_Y(Xi, Y) / H(Xi);
}




/*
X有k个社团
				  1    _k_   
H(X|Y)_norm =   -----  \  `  H(Xi|Y)_norm
				  k    /__,   
				       i=1
*/
double Communities::H_X_given_Y_norm(Communities & X, Communities & Y)
{
	double res = 0;
	for (size_t i = 0; i < X.size(); ++i)
	{
		res += H_Xi_given_Y_norm(X.comms[i], Y);
	}
	res /= X.size();

	return res;
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
		printf("-----comm %u,size=%u-----\n", i, comms[i].nodes.size());
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

pair<double, int> Community::Precision(Communities & truth)
{
	if (truth.size() == 0)
		return make_pair(0,0);

	double res = 0;
	int index = 0;
	for (size_t i = 0; i < truth.size(); ++i)
	{
		vector<int> cap = Communities::intersection(this->nodes, truth.comms[i].nodes);
		vector<int> join = Communities::setunion(this->nodes, truth.comms[i].nodes);
		double d1 = cap.size();
		double d2 = join.size();
		double r = d1 / d2;
		if (r > res)
		{
			res = r;
			index = i;
		}
	}

	return make_pair(res, index);
}
