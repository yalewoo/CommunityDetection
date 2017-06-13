#include <cstdio>
#include <iostream>
#include "../Graph.h"



using std::cout;
using std::endl;

map<string, string> Graph::config;



#include <io.h>

vector<string> getfile(const char* dir)
{
	vector<string> res;
	_finddata_t fileDir;

	long lfDir;

	if ((lfDir = _findfirst(dir, &fileDir)) == -1l)
		printf("No file is found\n");
	else {
		printf("file list:\n");
		do {
			//printf("%s\n", fileDir.name);
			string name = fileDir.name;
			res.push_back(name);
		} while (_findnext(lfDir, &fileDir) == 0);
	}
	_findclose(lfDir);

	return res;
}


void calc(Communities &detected, Communities &truth, string name, FILE * fp)
{


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



		fprintf(fp, "%s,%lf,%lf,%lf,%lf,%lf,%lf,%lf\n", name.c_str(), nmi, f1, p, r, jf1, jp, jr);

		printf("%s ok\n", name.c_str());

}

int main(int argc, char *argv[])
{
	


	Graph::loadConfig("F:/Project/CommunityDetection/config.txt");


	

	string graph_path;
	if (argc >= 2)
		graph_path = argv[1];
	else
		graph_path = "F:/HICODE_SUB/facebook/Smith60/";

	vector<string> ds;
	string detected_path;
	if (argc >= 3)
		detected_path = argv[2];
	else
		detected_path = "F:/facebook100/Smith60/";
	ds = getfile((detected_path + "*.gen").c_str());

	//Communities::mkdir("result/");

	FILE * fp;
	



	Communities l1, l2, sub1, sub2;
	l1.load(graph_path + "hicode/maxmodlayer1.gen");
	l2.load(graph_path + "hicode/maxmodlayer2.gen");
	sub1.load(graph_path + "sub/sub1.gen");
	sub2.load(graph_path + "sub/sub2.gen");



	string outdir = "f1/";
	Communities::mkdir(outdir);

	Communities::mkdir("facebook/");

	//truth1_1
	fp = fopen("facebook/l1.csv", "w");
	fprintf(fp, "l1:%d communities\n", l1.size());
	fprintf(fp, "Truth,NMI,F1,P,R,JF1,JP,JR\n");
	for (size_t i = 0; i < ds.size(); ++i)
	{
		Communities detected;
		detected.load(detected_path + ds[i]);
		calc(detected, l1, ds[i], fp);
	}
	fclose(fp);

	fp = fopen("facebook/l2.csv", "w");
	fprintf(fp, "l2:%d communities\n", l2.size());
	fprintf(fp, "Truth,NMI,F1,P,R,JF1,JP,JR\n");
	for (size_t i = 0; i < ds.size(); ++i)
	{
		Communities detected;
		detected.load(detected_path + ds[i]);
		calc(detected, l2, ds[i], fp);
	}
	fclose(fp);

	fp = fopen("facebook/sub1.csv", "w");
	fprintf(fp, "sub1:%d communities\n", sub1.size());
	fprintf(fp, "Truth,NMI,F1,P,R,JF1,JP,JR\n");
	for (size_t i = 0; i < ds.size(); ++i)
	{
		Communities detected;
		detected.load(detected_path + ds[i]);
		calc(detected, sub1, ds[i], fp);
	}
	fclose(fp);

	fp = fopen("facebook/sub2.csv", "w");
	fprintf(fp, "sub2:%d communities\n", sub2.size());
	fprintf(fp, "Truth,NMI,F1,P,R,JF1,JP,JR\n");
	for (size_t i = 0; i < ds.size(); ++i)
	{
		Communities detected;
		detected.load(detected_path + ds[i]);
		calc(detected, sub2, ds[i], fp);
	}
	fclose(fp);






	printf("------------\ndone\n");
	return 0;
}