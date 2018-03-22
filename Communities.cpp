#include "Communities.h"

#include <cctype>
#include <cstring>
#include "Communities.h"

#include <fstream>
#include <string>
#include <sstream>
#include <iostream>

//#include <QDebug>

#include <cmath>
double log2(double x)
{
	return log(x) / log(2);
}

#include "Graph.h"

#include "ylog.h"

using std::ifstream;
using std::string;
using std::stringstream;
using std::cout;
using std::endl;
using std::min;
using std::make_pair;



bool Community::isSubsetOf(Community & c)
{
	vector<int> res = Communities::intersection(nodes, c.nodes);
	if (res.size() == nodes.size() && res.size() < c.nodes.size())
		return true;
	else
		return false;
}

int Communities::nodesNum()
{
	set<int> s;
	for (size_t i = 0; i < comms.size(); ++i)
	{
		for (size_t j = 0; j < comms[i].size(); ++j)
		{
			s.insert(comms[i].nodes[j]);
		}
	}

	return s.size();
}

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
	if (!f)
		return;


	while (!f.eof())
	{
		string s;
		getline(f, s);


		stringstream stream;
		stream.str(s);
		int id;
		Community c;
		while (stream >> id)
		{
			c.add(id);
		}
		c.sort();
        if (c.nodes.size() >= 2)
			addCommunity(c);
	}
	//removeSmallComm(2);
}

void Communities::loadInfomap(const char * fn)
{
	clear();


	FILE *fp = fopen(fn, "r");
	if (!fp)
		return;
	char buff[512];
	fgets(buff, 512, fp);
	fclose(fp);
	int i = strlen(buff);
	while (buff[i] != 'n')
		--i;
	++i; ++i;
	int levels;
	sscanf(buff + i, "%d", &levels);

	for (int level = 0; level < levels - 1; ++level)
	{
		vector<Community> vc;
		layers.push_back(vc);

		fp = fopen(fn, "r");
		fgets(buff, 512, fp);
		fgets(buff, 512, fp);


		int n;
		int lastn = 1;
		int node;

		Community c;
		while (fgets(buff, 512, fp))
		{
			//定位到倒数第二个数字 三层时1:2:3 会读出2
			int i = 0;
			int colonnum = 0;
			while (colonnum < level)
			{
				while (buff[i] != ':')
					++i;
				++colonnum;
			}
			if (level > 0)	++i;
			//社团编号
			sscanf(buff + i, "%d", &n);

			if (n != lastn)
			{
				layers[level].push_back(c);
				c.clear();
			}
			lastn = n;

			i = strlen(buff);
			while (buff[i] != ' ')
				--i;
			++i;
			//节点号
			sscanf(buff + i, "%d", &node);

			c.add(node);


		}
		layers[level].push_back(c);
		c.clear();

		removeSmallComm(layers[level], 2);

		fclose(fp);

	}

	comms = layers[0];

	
}

void Communities::loadInfomap0135(const char * fn)
{
	clear();


	FILE *fp = fopen(fn, "r");
	if (!fp)
		return;
	char buff[512];
	fgets(buff, 512, fp);
	fgets(buff, 512, fp);
	fclose(fp);
	int len = strlen(buff);
	int i = 0;
	int levels = 1;
	for (; i < len; ++i)
	{
		if (buff[i] == ':')
			++levels;
	}


	for (int level = 0; level < levels - 1; ++level)
	{
		vector<Community> vc;
		layers.push_back(vc);

		fp = fopen(fn, "r");
		fgets(buff, 512, fp);


		int n;
		int lastn = 1;
		int node;

		Community c;
		while (fgets(buff, 512, fp))
		{
			//定位到倒数第二个数字 三层时1:2:3 会读出2
			int i = 0;
			int colonnum = 0;
			while (colonnum < level)
			{
				while (buff[i] != ':')
					++i;
				++colonnum;
			}
			if (level > 0)	++i;
			//社团编号
			sscanf(buff + i, "%d", &n);

			if (n != lastn)
			{
				layers[level].push_back(c);
				c.clear();
			}
			lastn = n;

			i = strlen(buff);
			while (buff[i] != ' ')
				--i;

			++i;
			++i;
			//节点号
			sscanf(buff + i, "%d", &node);

			c.add(node);


		}
		layers[level].push_back(c);
		c.clear();

		removeSmallComm(layers[level], 2);

		fclose(fp);

	}

	comms = layers[0];


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
		stream.str(s);
		int id;
		Community c;
		while (stream >> id)
		{
			c.add(id);
		}
		c.sort();
		if (c.nodes.size() > 2)
			addCommunity(c);

	}
	//removeSmallComm(1);
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
		stream.str(s);
		int id;
		Community c;
		while (stream >> id)
		{
			c.add(id);
		}
		c.sort();
		if (c.nodes.size() > 2)
			addCommunity(c);
	}
	//removeSmallComm(1);
	layers.push_back(comms);
	++layer;


	while (true)
	{
		char tpname[256];
		sprintf(tpname, "%s%d", fn, layer);

		ifstream f(tpname, std::ios::in);
		if (!f)
			break;

		vector<Community> vc;
		while (!f.eof())
		{
			string s;
			getline(f, s);

			if (s.size() > 0 && s[0] == '#')
				continue;

			stringstream stream;
			stream.str(s);
			int id;
			Community c;
			while (stream >> id)
			{
				c.add(id);
			}
			c.sort();
			if (c.nodes.size() > 2)
			{
				vc.push_back(c);
			}
		}


		
		layers.push_back(vc);
		++layer;
	}
	
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
		stream.str(s);
		int id;
		stream >> id;	//第一个数字是社团号舍弃
		Community c;
		while (stream >> id)
		{
			c.add(id);
		}
		c.sort();
		if (c.nodes.size() > 2)
			addCommunity(c);
	}
	//removeSmallComm(1);
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
		stream.str(s);
		int id;
		stream >> id;	//第一个数字是社团号舍弃
		Community c;
		while (stream >> id)
		{
			c.add(id);
		}
		c.sort();
		if (c.nodes.size() > 2)
			addCommunity(c);
	}
	//removeSmallComm(1);
}

void Communities::loadMod(const char * fn, int level)
{
	ylog("Communities::loadMod begin");
	string hierarchy = "\"" + Graph::config["Mod_dir"] + "hierarchy\"";

	Graph::cmd(hierarchy + " mod.result -n > mod_num.txt");

	FILE * fp = fopen("mod_num.txt", "r");
	if (!fp) {
		ylog("Communities::loadMod openfile mod_num.txt failed");
		return;
	}
	char buff[256];
	fgets(buff, 256, fp);
	int i = 0;
	for (; i < 256; ++i)
	{
		if (buff[i] == ':')
			break;
	}
	ylog("Communities::loadMod:find: ok i=");
	ylog(i);

	int n;
	sscanf(buff + i + 1, "%d", &n);
	printf("levels = %d\n", n);

	fclose(fp);

	for (i = n - 1; i > 0; --i)
	{
		ylog("Communities::loadMod level=");
		ylog(i);
		sprintf(buff, " mod.result -l %d > mod_layer.txt", i);
		Graph::cmd(hierarchy + buff);

		vector<Community> vc;
		layers.push_back(vc);
		

		loadCid(layers[layer], "mod_layer.txt");
		++layer;
	}
	comms = layers[0];

	ylog("Communities::loadMod end");

}

void Communities::loadCid(vector< Community > & comm, const char * fn)
{
	ylog("Communities::loadCid begin");
	ifstream f1(fn, std::ios::in);
	if (!f1) return;

	string s;
	getline(f1, s);

	stringstream stream3;
	stream3.str(s);
	int node_id;
	int comm_id;
	stream3 >> node_id >> comm_id;
	int last;
	int maxnode = 0;

	while (!f1.eof())
	{

		getline(f1, s);
		stringstream stream2;
		stream2 << s;
		stream2 >> node_id >> comm_id;

		maxnode = max(maxnode, node_id);
		if (node_id == 0)
			break;
	}
	f1.close();


	comm.clear();
	vector<int> cid(maxnode + 1);


	ifstream f(fn, std::ios::in);



	getline(f, s);

	stringstream stream;
	stream.str(s);

	stream >> node_id >> comm_id;


	while (!f.eof())
	{
		cid[node_id] = comm_id;

		last = comm_id;

		getline(f, s);
		stringstream stream2;
		stream2 << s;
		stream2 >> node_id >> comm_id;

		if (node_id == 0)
			break;

	}
	f.close();
	getCommsByCid(comm, cid);
	removeSmallComm(comm, 2);

	ylog("Communities::loadCid end");
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

double Communities::calcNMI(Communities & cs, string outdir)
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
	if (!outdir.empty())
	{
		FILE * fp = fopen((outdir + "NMI.csv").c_str(), "w");
		if (fp)
		{
			fprintf(fp,"NMI\n");
			fprintf(fp,"%lf", res);
		}
		fclose(fp);
	}
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
pair<double,double> Communities::Jaccard(Communities & Detected, Communities & truth, vector<int>& v_index, vector<double> &v_value,FILE * fp)
{
	v_index.clear();
	v_value.clear();
	v_index.resize(Detected.size());
	v_value.resize(Detected.size());
	double m = Detected.sizeOfCommsSum();
	double res = 0;
	double unweightedres = 0;
	
	for (size_t i = 0; i < Detected.size(); ++i)
	{
		if (fp)
			fprintf(fp, "%d,", i+1);
		pair<double, int> p = Detected.comms[i].JaccardPrecision(truth, fp);
		v_value[i] = p.first;
		res += p.first * Detected.comms[i].size() / m;
		unweightedres += p.first;
		v_index[i] = p.second;
	}
	unweightedres /= (double)Detected.size();
	if (fp)
	{
		fprintf(fp, "Weighted Average,%lf\n", res);
		fprintf(fp, "Average,%lf\n", unweightedres);
		fclose(fp);
	}
		

	return make_pair(res,unweightedres);
}

pair<double, double> Communities::JaccardRecall(Communities & Detected, Communities & truth, vector<int>& v_index, vector<double> &v_value, string dir)
{
	
	string fn = dir + "JaccardRecall.csv";
	FILE * fp = fopen(fn.c_str(), "w");
	if (fp)
		fprintf(fp, "Truth id,Truth size,Detected id,Detected size,Recall,Truth left,intersect,Detected left\n");
	return Jaccard(truth, Detected, v_index, v_value,fp);
}
pair<double, double> Communities::JaccardPrecision(Communities & Detected, Communities & truth, vector<int>& v_index, vector<double> &v_value, string dir)
{

	string fn = dir + "JaccardPrecision.csv";
	FILE * fp = fopen(fn.c_str(), "w");
	if (fp)
		fprintf(fp, "Detected id,Detected size,Truth id,Truth size,Precision,Detected left,intersect,Truth left\n");
	return Jaccard(Detected, truth, v_index, v_value,fp);

}

pair<double, double> Communities::JaccardF1Score(Communities & Detected, Communities & truth, string dir)
{
	vector<int> tmp;
	vector<double> tmpd;
	


	string fn = dir + "F1FJ.csv";
	FILE * fp = fopen(fn.c_str(), "w");
	if (fp)
		fprintf(fp, "WeightedJaccardF1, Weighted Jaccard Precision, Weighted Jaccard Recall,JaccardF1, Jaccard Precision, Jaccard Recall\n");

	pair<double, double> P_pair;
	pair<double, double> R_pair;
	P_pair = JaccardPrecision(Detected, truth, tmp, tmpd, dir);
	R_pair = JaccardRecall(Detected, truth, tmp, tmpd, dir);
	double P = P_pair.first;
	double R = R_pair.first;
	double Weighted = 2 * P * R / (P + R);
	if (P + R == 0) Weighted = 0;
	if (fp)
		fprintf(fp, "%lf,%lf,%lf,", Weighted, P, R);
	P = P_pair.second;
	R = R_pair.second;
	double UnWeighted = 2 * P * R / (P + R);
	if (P + R == 0) UnWeighted = 0;
	if (fp)
		fprintf(fp, "%lf,%lf,%lf", UnWeighted, P, R);
	
	fclose(fp);
	return make_pair(Weighted, UnWeighted);
}

pair<double, double> Communities::PRGeneral(Communities & Detected, Communities & truth, vector<int> & v_index, vector<double> &v_value, FILE * fp)
{
	v_index.clear();
	v_value.clear();
	v_index.resize(Detected.size());
	v_value.resize(Detected.size());
	double m = Detected.sizeOfCommsSum();
	double res = 0;
	double unweightedres = 0;

	for (size_t i = 0; i < Detected.size(); ++i)
	{
		if (fp)
			fprintf(fp, "%d,", i + 1);
		pair<double, int> p = Detected.comms[i].Precision(truth, fp);
		v_value[i] = p.first;
		res += p.first * Detected.comms[i].size() / m;
		unweightedres += p.first;
		v_index[i] = p.second;
	}
	unweightedres /= (double)Detected.size();
	if (fp)
	{
		fprintf(fp, "Weighted Average,%lf\n", res);
		fprintf(fp, "Average,%lf\n", unweightedres);
		fclose(fp);
	}
	

	return make_pair(res, unweightedres);
}
pair<double, double> Communities::Recall(Communities & Detected, Communities & truth, vector<int> & v_index, vector<double> &v_value, string dir)
{

	string fn = dir + "Recall.csv";
	FILE * fp = fopen(fn.c_str(), "w");
	if (fp)
		fprintf(fp, "Truth id,Truth size,Detected id,Detected size,Recall,intersect\n");
	return PRGeneral(truth, Detected, v_index, v_value, fp);
}
pair<double, double> Communities::Precision(Communities & Detected, Communities & truth, vector<int> & v_index, vector<double> &v_value, string dir)
{

	string fn = dir + "Precision.csv";
	FILE * fp = fopen(fn.c_str(), "w");
	if (fp)
		fprintf(fp, "Detected id,Detected size,Truth id,Truth size,Precision,intersect\n");
	return PRGeneral(Detected, truth, v_index, v_value, fp);
}
pair<double, double> Communities::PR(Communities & Detected, Communities & truth, string dir)
{
	vector<int> tmp;
	vector<double> tmpd;


	string fn = dir + "F1.csv";
	FILE * fp = fopen(fn.c_str(), "w");
	if (fp)
		fprintf(fp, "WeightedF1, Weighted Precision, Weighted Recall,F1, Precision, Recall\n");

	pair<double, double> P_pair;
	pair<double, double> R_pair;
	P_pair = Precision(Detected, truth, tmp, tmpd, dir);
	R_pair = Recall(Detected, truth, tmp, tmpd, dir);
	double P = P_pair.first;
	double R = R_pair.first;
	double Weighted = 2 * P * R / (P + R);
	if (P + R == 0) Weighted = 0;
	if (fp)
		fprintf(fp, "%lf,%lf,%lf,", Weighted, P, R);
	P = P_pair.second;
	R = R_pair.second;
	double UnWeighted = 2 * P * R / (P + R);
	if (P + R == 0) UnWeighted = 0;
	if (fp)
		fprintf(fp, "%lf,%lf,%lf", UnWeighted, P, R);

	fclose(fp);
    return make_pair(Weighted, UnWeighted);
}

double Communities::f1(Community &c1, Community &c2)
{
    vector<int> cap = Communities::intersection(c1.nodes, c2.nodes);
    double join = cap.size();
    double p = join / c2.size();
    double r = join / c1.size();
    double f = 2 * p * r / (p + r);
    return f;
}

pair<double, int> Communities::f1(Community &c1, Communities &cs)
{
    double res = 0;
    int i;
    int index = 0;
    for (i = 0; i < cs.size(); ++i)
    {
        double f = f1(c1, cs.comms[i]);
        if (f > res)
        {
            res = f;
            index = i;
        }
    }
    return make_pair(res, index);
}

pair<double, int> Communities::p(Community &c1, Communities &cs)
{
    double res = 0;
    int i;
    int index = 0;
    for (i = 0; i < cs.size(); ++i)
    {
        Community & c2 = cs.comms[i];
        vector<int> cap = Communities::intersection(c1.nodes, c2.nodes);
        double join = cap.size();
        double p = join / c1.size();


        if (p > res)
        {
            res = p;
            index = i;
        }
    }
    return make_pair(res, index);
}

double Communities::f1(Communities &truth, Communities &detected)
{
    double res = 0;
    double count = 0;
    for (int i = 0; i < truth.size(); ++i)
    {
        res += f1(truth.comms[i], detected).first;
        ++count;
    }
    //qDebug() << count;
    return res / count;
}

double Communities::wf1(Communities &truth, Communities &detected)
{
    double res = 0;
    double count = 0;
    double m = truth.sizeOfCommsSum();

    for (int i = 0; i < truth.size(); ++i)
    {
        res += f1(truth.comms[i], detected).first  * truth.comms[i].size() / m;
        ++count;
    }

    return res;
}


double Communities::findSimilar(Community &c1, Community &c2)
{
    vector<int> cap = Communities::intersection(c1.nodes, c2.nodes);
    vector<int> u = Communities::setunion(c1.nodes, c2.nodes);
    double join = cap.size();
    double usize = u.size();
    double f = join / usize;
    return f;
}
pair<double, int> Communities::findSimilar(Community &c1, Communities &cs)
{
    double res = 0;
    int i;
    int index = 0;
    for (i = 0; i < cs.size(); ++i)
    {
        double f = findSimilar(c1, cs.comms[i]);
        if (f > res)
        {
            res = f;
            index = i;
        }
    }
    return make_pair(res, index);
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



void Communities::getCommsByCid(vector< Community > & comm, const vector<int> &cid)
{
	ylog("Communities::getCommsByCid begin");

	comm.clear();
	int max_comm_id = *std::max_element(cid.begin(), cid.end());
	comm.resize(max_comm_id + 1);
	for (int i = 0; i < cid.size(); ++i)
	{
		comm[cid[i]].add(i);
	}
	removeSmallComm(comms, 1);

	ylog("Communities::getCommsByCid end");
}

void Communities::removeSmallComm(vector< Community > & comm, int size)
{
	ylog("Communities::removeSmallComm begin");

	sort(comm.begin(), comm.end());
	for (auto it = comm.begin(); it != comm.end();)
	{
		if (it->size() <= size)
			it = comm.erase(it);    //删除元素，返回值指向已删除元素的下一个位置    
		else
		{
			it->sort();
			++it;    //指向下一个位置
		}
			
	}

	ylog("Communities::removeSmallComm end");

}

void Communities::removeBigComm(vector<Community> &comm, int size)
{
    ylog("Communities::removeSmallComm begin");

    sort(comm.begin(), comm.end());
    for (auto it = comm.begin(); it != comm.end();)
    {
        if (it->size() >= size)
            it = comm.erase(it);    //删除元素，返回值指向已删除元素的下一个位置
        else
        {
            it->sort();
            ++it;    //指向下一个位置
        }

    }

    ylog("Communities::removeBigComm end");
}

string Communities::print(bool show_detail, bool show_nodes)
{
	string res;
	char buff[256];
	printf("-------------------------\n");
	printf("Modularity: %lf\n", Q);
	printf("%d communities:\n", comms.size());

	sprintf(buff, "-------------------------\n");
	res += buff;
	sprintf(buff, "Modularity: %lf\n", Q);
	res += buff;
	sprintf(buff, "%d communities:\n", comms.size());
	res += buff;
    sprintf(buff, "%d nodes:\n", nodesNum());
    res += buff;
	if (show_detail)
	{
		for (size_t i = 0; i < comms.size(); ++i)
		{
			printf("-----comm %u,size=%u-----\n", i+1, comms[i].nodes.size());
			sprintf(buff, "-----comm %u,size=%u-----\n", i+1, comms[i].nodes.size());
			res += buff;
			if (show_nodes)
			{
				for (size_t j = 0; j < comms[i].nodes.size(); ++j)
				{
					printf("%d ", comms[i].nodes[j]);
					sprintf(buff, "%d ", comms[i].nodes[j]);
					res += buff;
				}
				printf("\n");
				sprintf(buff, "\n");
				res += buff;
			}
		
		
		}
	}

	return res;

}
bool Communities::save(string fn)
{
	return save(fn.c_str());
}
Communities Communities::merge(Communities & other)
{
	Communities res;
	res.max_node_id = max(max_node_id, other.max_node_id);
	res.min_node_id = min(min_node_id, other.min_node_id);
	size_t i = 0, j = 0;
	while (i < comms.size() && j < other.comms.size())
	{
		if (comms[i] < other.comms[j])
		{
			res.addCommunity(comms[i]);
			++i;
		}
		else
		{
			res.addCommunity(other.comms[j]);
			++j;
		}
	}
	while (i < comms.size())
	{
		res.addCommunity(comms[i]);
		++i;
	}
	while (j < other.comms.size())
	{
		res.addCommunity(other.comms[j]);
		++j;
	}


	return res;
}
bool Communities::save(const char * fn)
{
	ylog("save begin");
	sort(comms.begin(), comms.end());

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


	char buff[256];
	for (int l = 1; l < layer; ++l)
	{
		sprintf(buff, "%s_layer%d", fn, l);

		fp = fopen(buff, "w");
		if (!fp) return false;
		vector<Community> & comm = layers[l];
		for (size_t i = 0; i < comm.size(); ++i)
		{
			for (size_t j = 0; j < comm[i].nodes.size(); ++j)
			{
				fprintf(fp, "%d ", comm[i].nodes[j]);
			}
			fprintf(fp, "\n");
		}
		fclose(fp);
	}

	ylog("save end");
	
	return true;
}

pair<double, int> Community::JaccardPrecision(Communities & truth, FILE * fp)
{
	if (truth.size() == 0)
		return make_pair(0,0);

	double res = 0;
	int index = 0;
	int inter = 0;
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
			inter = cap.size();
		}
	}
	if (fp)
	{
		//Detected size,Truth id,Truth size,Precision,Detected left,intersect,Truth left
		fprintf(fp, "%d,%d,%d,%lf,%d,%d,%d\n", this->size(),index+1,truth.comms[index].size(),
			res, this->size()-inter, inter, truth.comms[index].size()-inter);
	}

	return make_pair(res, index);
}
pair<double, int> Community::Precision(Communities & truth, FILE * fp)
{
	if (truth.size() == 0)
		return make_pair(0, 0);

	double res = 0;
	int index = 0;
	int inter = 0;
	for (size_t i = 0; i < truth.size(); ++i)
	{
		vector<int> cap = Communities::intersection(this->nodes, truth.comms[i].nodes);
		vector<int> & join = this->nodes;
		double d1 = cap.size();
		double d2 = join.size();
		double r = d1 / d2;


		if (r > res)
		{
			res = r;
			index = i;
			inter = cap.size();
		}
	}
	if (fp)
	{
		//Detected size,Truth id,Truth size,Precision,intersect
		fprintf(fp, "%d,%d,%d,%lf,%d\n", this->size(), index + 1, truth.comms[index].size(),
			res, inter);
	}

	return make_pair(res, index);
}
