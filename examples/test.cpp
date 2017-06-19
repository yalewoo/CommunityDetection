#include <cstdio>
#include <iostream>
#include "../Graph.h"
#include "../Config.h"


using std::cout;
using std::endl;

map<string, string> Graph::config;

template <typename T>
void showVector(vector<T> & v, string name = "vector", int id = 0)
{
	cout << name << " " << id << " (size=" << v.size() << ") : " << endl;
	for (size_t i = 0; i < v.size(); ++i)
		cout << v[i] << "\n";
	cout << endl;
}






struct Csv2rec {
	vector<string> header;
	vector<double> xs;
	vector<vector<double> > data;

	Communities layer1, layer2;


	int line = -1;

	Csv2rec() {
		header.push_back(string("x"));

		addheader("layer1");
		addheader("layer2");
		addline(0);

	}
	void init(Communities & truth)
	{
		double nmi1, nmi2;
		nmi1 = layer1.calcNMI(truth);
		nmi2 = layer2.calcNMI(truth);

		adddata(nmi1);
		adddata(nmi2);
	}
	void addNMI(int line, Communities & truth)
	{
		double nmi1 = layer1.calcNMI(truth);
		double nmi2 = layer2.calcNMI(truth);
		addline(line + 1);
		adddata(nmi1);
		adddata(nmi2);
	}
	void addheader(char * s)
	{
		header.push_back(string(s));
	}
	void addline(double x)
	{
		xs.push_back(x);
		data.push_back(vector<double>());
		++line;
	}
	void adddata(double n)
	{
		data[line].push_back(n);
	}
	bool save(const char * fn)
	{
		FILE * fp = fopen(fn, "w");
		if (!fp) return false;



		size_t i;
		fprintf(fp, "%s", header[0].c_str());
		for (i = 1; i < header.size(); ++i)
		{
			fprintf(fp, ",%s", header[i].c_str());
		}
		fprintf(fp, "\n");


		for (i = 0; i < data.size(); ++i)
		{
			fprintf(fp, "%lf", xs[i]);
			for (size_t j = 0; j < data[i].size(); ++j)
			{
				fprintf(fp, ",%lf", data[i][j]);
			}
			fprintf(fp, "\n");
		}

		fclose(fp);

		return true;
	}
};



int main(int argc, char *argv[])
{
	Graph::loadConfig("F:/Project/CommunityDetection/config.txt");

	Config con;
	con.updateConfig("F:/HICODE_SUB/syn/testGraph/hicode.config");

	cout << con["truth_path"] << endl;
	cout << con["truth_path"] << endl;
	cout << con["truth_path"] << endl;

	cout << con["truth_path"].empty() << endl;

	showVector(con.getValue("base_alg"));
	

	printf("------------\ndone\n");
	return 0;
}