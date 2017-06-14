#include <cstdio>
#include <iostream>
#include "../Graph.h"



using std::cout;
using std::endl;

map<string, string> Graph::config;

string hicode_path;
string sub_path;


string getName(string path, int layer, int iter)
{
	int i = 1;
	while (true)
	{
		char buff[256];
		sprintf(buff, (hicode_path + "layer%d_%d.txt_layer%d").c_str(), layer, iter, i);
		string p = path + buff;
		FILE * fp = fopen(p.c_str(), "r");
		if (!fp)
			break;
		fclose(fp);
		++i;
	}
	--i;
	char buf[256];
	if (i == 0)
		sprintf(buf, (hicode_path + "layer%d_%d.txt").c_str(), layer, iter);
	else
		sprintf(buf, (hicode_path + "layer%d_%d.txt_layer%d").c_str(), layer, iter, i);
	string s = buf;
	return s;
}
string getName2(string path, int layer)
{
	int i = 1;
	while (true)
	{
		char buff[256];
		sprintf(buff, (hicode_path + "maxmodlayer%d.gen_layer%d").c_str(), layer, i);
		string p = path + buff;
		FILE * fp = fopen(p.c_str(), "r");
		if (!fp)
			break;
		fclose(fp);
		++i;
	}
	--i;
	char buf[256];
	if (i == 0)
		sprintf(buf, (hicode_path + "maxmodlayer%d.gen").c_str(), layer);
	else
		sprintf(buf, (hicode_path + "maxmodlayer%d.gen_layer%d").c_str(), layer, i);
	string s = buf;
	return s;
}
struct calcReturn {
	double nmi;
	double f1;
	double jf1;
};
calcReturn calc(Communities &truth, int len, Communities * cs[], char *names[], FILE * fp)
{
	calcReturn res;
	double max_nmi = 0;
	double max_f1 = 0;
	double max_jf1 = 0;
	for (int i = 0; i < len; ++i)
	{
		Communities detected = *(cs[i]);

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



		fprintf(fp, "%s,%lf,%lf,%lf,%lf,%lf,%lf,%lf\n", names[i],nmi, f1, p, r, jf1, jp, jr);
		max_nmi = max(max_nmi, nmi);
		max_f1 = max(max_f1, f1);
		max_jf1 = max(max_jf1, jf1);
	}

	res.nmi = max_nmi;
	res.f1 = max_f1;
	res.jf1 = max_jf1;
	return res;
}

int main(int argc, char *argv[])
{


	Graph::loadConfig("F:/Project/CommunityDetection/config.txt");


	Communities truth1_1, truth1_2, truth2_1, truth2_2;

	string graph_path;
	if (argc >= 2)
		graph_path = argv[1];
	else
		graph_path = "F:/HICODE_SUB/syn/3000_ori/";

	string basealg;
	if (argc >= 3)
		basealg = argv[2];
	else
		basealg = "mod";

	hicode_path = "hicode_" + basealg + "/";
	sub_path = "sub_" + basealg + "/";

	string outdir = "statistics_" + basealg + "/";


	Communities::mkdir(outdir);

	FILE * fp = fopen((outdir + "Statistics.csv").c_str(), "w");
	FILE * f2 = fopen((outdir + "maxresult.txt").c_str(), "w");
	FILE * f3 = fopen((outdir + "maxresult_f1.txt").c_str(), "w");
	double max_nmi, max_f1;
	calcReturn calcr;

	truth1_1.load(graph_path + "truth1_1.gen");
	truth1_2.load(graph_path + "truth1_2.gen");
	truth2_1.load(graph_path + "truth2_1.gen");
	truth2_2.load(graph_path + "truth2_2.gen");

	string s_l1mod2;


	Communities l1, l2, sub1, sub2;

	l1.load(graph_path + hicode_path + "maxmodlayer1.gen");
	sub1.load(graph_path + sub_path + "sub1_1.gen");

	l2.load(graph_path + hicode_path + "maxmodlayer2.gen");
	sub2.load(graph_path + sub_path + "sub2_1.gen");



	outdir = "f1/";
	Communities::mkdir(outdir);

	//truth1_1
	Communities * cs11[5];
	cs11[0] = &l1;
	cs11[1] = &sub1;
	cs11[2] = &l2;
	cs11[3] = &sub2;

	char *name11[4] = {"l1", "sub1", "l2", "sub2"};
	fprintf(fp, "truth1_1:%d communities\n", truth1_1.size());
	fprintf(fp, "Detected,NMI,F1,P,R,JF1,JP,JR\n");
	calcr = calc(truth1_1, 4, cs11, name11, fp);
	fprintf(f2, "%f\n", calcr.nmi);
	fprintf(f3, "%f\n", calcr.f1);

	//truth1_2
	fprintf(fp, "truth1_2:%d communities\n", truth1_2.size());
	fprintf(fp, "Detected,NMI,F1,P,R,JF1,JP,JR\n");
	calcr = calc(truth1_2, 4, cs11, name11, fp);
	fprintf(f2, "%f\n", calcr.nmi);
	fprintf(f3, "%f\n", calcr.f1);
	//truth2_1

	fprintf(fp, "truth2_1:%d communities\n", truth2_1.size());
	fprintf(fp, "Detected,NMI,F1,P,R,JF1,JP,JR\n");
	calcr = calc(truth2_1, 4, cs11, name11, fp);
	fprintf(f2, "%f\n", calcr.nmi);
	fprintf(f3, "%f\n", calcr.f1);
	//truth2_2
	fprintf(fp, "truth2_2:%d communities\n", truth2_2.size());
	fprintf(fp, "Detected,NMI,F1,P,R,JF1,JP,JR\n");
	calcr = calc(truth2_2, 4, cs11, name11, fp);
	fprintf(f2, "%f\n", calcr.nmi);
	fprintf(f3, "%f\n", calcr.f1);


	
	fclose(fp);
	fclose(f2);
	fclose(f3);




	printf("------------\ndone\n");
	return 0;
}