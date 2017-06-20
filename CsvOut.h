#include <vector>
#include <string>
using std::vector;
using std::string;

#include <cstdio>
#include "Graph.h"

#include "os.h"


struct Csv2rec {
	vector<string> header;
	vector<vector<double> > data;

	Csv2rec(int n, vector<vector<double> > &data2)
	{
		setHeader(n);
		setData(data2);
	}
	void setHeader(int nLayer)
	{
		header.push_back("x");
		for (int i = 1; i <= nLayer; ++i)
		{
			char buff[256];
			sprintf(buff, "layer%d", i);
			header.push_back(buff);
		}
	}
	void setData(vector<vector<double> > & d)
	{
		data = d;
	}
	bool save(string fn)
	{
		return save(fn.c_str());
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
			fprintf(fp, "%d", i);
			for (size_t j = 0; j < data[i].size(); ++j)
			{
				fprintf(fp, ",%lf", data[i][j]);
			}
			fprintf(fp, "\n");
		}

		fclose(fp);

		return true;
	}

	bool savePNG(string & fn, string & out)
	{
		string python = Graph::config["python3_exe"];
		string py = python + " " + Graph::config["py_plot_csv"] + " ";
		os::cmd(py + fn + " " + out);
		return true;
	}
};