#include <cstdio>
#include <iostream>
#include "../Graph.h"

#include "../os.h"

using std::cout;
using std::endl;

map<string, string> Graph::config;



void calc(Communities & truth, vector<Communities> & cs, vector<string> & cs_name, FILE * fp)
{

	for (size_t i = 0; i < cs.size(); ++i)
	{
		printf("process detected %u\n", i);
		Communities & detected = cs[i];

		double nmi = detected.calcNMI(truth);

		vector<int> v_index;
		vector<double> v_value;
		string outdir = "f1/";

		double p = Communities::Precision(detected, truth, v_index, v_value, outdir).first;
		double r = Communities::Recall(detected, truth, v_index, v_value, outdir).first;
		double f1 = Communities::F1Score(detected, truth, outdir).first;


		double jp = Communities::JaccardPrecision(detected, truth, v_index, v_value, outdir).first;
		double jr = Communities::JaccardRecall(detected, truth, v_index, v_value, outdir).first;
		double jf1 = Communities::JaccardF1Score(detected, truth, outdir).first;



		fprintf(fp, "%s,%lf,%lf,%lf,%lf,%lf,%lf,%lf\n", cs_name[i].c_str(),nmi, f1, p, r, jf1, jp, jr);

	}

}

int main(int argc, char *argv[])
{


	string graph_path;
	if (argc >= 2)
		graph_path = argv[1];
	else
		graph_path = "F:/HICODE_SUB/facebook/UCSC68/";

	FILE * fp = fopen((graph_path + "detected_files.txt").c_str(), "r");
	if (!fp)
		printf("fopen detected_files.txt failed");
	vector<string> detected_path;
	vector<Communities> detected;
	char buff[1024];
	while (!feof(fp))
	{
		fgets(buff, 1000, fp);
		string sfn = buff;
		if (sfn.size() > 1 && sfn[sfn.size() - 1] == '\n') sfn = sfn.substr(0, sfn.size() - 1);
		detected_path.push_back(sfn);

		Communities cs;
		cs.load(graph_path + sfn);
		//cs.print();
		detected.push_back(cs);
	}
	fclose(fp);

	fp = fopen((graph_path + "truth_files.txt").c_str(), "r");
	if (!fp)
		printf("fopen truth_files.txt failed");
	vector<string> truth_path;
	vector<Communities> truth;
	while (!feof(fp))
	{
		fgets(buff, 1000, fp);
		string sfn = buff;
		if (sfn.size() > 1 && sfn[sfn.size() - 1] == '\n') sfn = sfn.substr(0, sfn.size() - 1);
		truth_path.push_back(sfn);

		Communities cs;
		cs.load(graph_path + sfn);
		//cs.print();
		truth.push_back(cs);
	}
	fclose(fp);

	//showVector(truth_path);



	string outdir = "statistic/";
	os::mkdir(outdir);
	string outdir2 = "f1/";
	os::mkdir(outdir2);

	for (int truth_i = 0; truth_i < truth.size(); ++truth_i)
	{
		printf("process truth %d..\n", truth_i);

		string tpath = truth_path[truth_i];
		for (int i = 0; i < tpath.size(); ++i)
		{
			if (tpath[i] == '\\' || tpath[i] == '/')
				tpath[i] = '_';
		}
		fp = fopen((outdir + tpath + ".csv").c_str(), "w");
		if (fp == 0)
		{
			printf("open %s failed\n", (outdir + tpath + ".csv").c_str());
		}

		fprintf(fp, "Detected,NMI,F1,P,R,JF1,JP,JR\n");

		calc(truth[truth_i], detected, detected_path, fp);

		fclose(fp);

	}

	os::moveDir(outdir, graph_path);

	printf("------------\ndone\n");
	return 0;
}