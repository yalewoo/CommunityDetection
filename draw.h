#include <vector>
#include <string>
using std::vector;
using std::string;


struct Csv2rec {
	vector<string> header;
	vector<double> xs;
	vector<vector<double> > data;


	int line = -1;

	Csv2rec() {
		header.push_back(string("x"));
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