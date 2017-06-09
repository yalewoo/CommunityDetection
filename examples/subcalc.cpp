#include <cstdio>
#include <iostream>
#include "../Graph.h"



using std::cout;
using std::endl;

map<string, string> Graph::config;


string getName(string path, int layer, int iter)
{
	int i = 1;
	while (true)
	{
		char buff[256];
		sprintf(buff, "hicode/layer%d_%d.txt_layer%d", layer, iter, i);
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
		sprintf(buf, "hicode/layer%d_%d.txt", layer, iter);
	else
		sprintf(buf, "hicode/layer%d_%d.txt_layer%d", layer, iter, i);
	string s = buf;
	return s;
}

void calc(Communities &truth, int len, Communities * cs[], char *names[], FILE * fp)
{

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
	}
}

int main(int argc, char *argv[])
{


	Graph::loadConfig("F:/Project/CommunityDetection/config.txt");


	Communities truth1_1, truth1_2, truth2_1, truth2_2;

	string graph_path;
	if (argc == 2)
		graph_path = argv[1];
	else
		graph_path = "F:/HICODE_SUB/syn/3000/";

	//Communities::mkdir("result/");

	FILE * fp = fopen("Statistics.csv", "w");



	truth1_1.load(graph_path + "truth1_1.gen");
	truth1_2.load(graph_path + "truth1_2.gen");
	truth2_1.load(graph_path + "truth2_1.gen");
	truth2_2.load(graph_path + "truth2_2.gen");

	string s_l1mod2;


	Communities mod0, mod2, l1himod0, l1himod2, l1himod0sub;
	mod0.load(graph_path + "hicode/layer1_0.txt");
	string l1mod2path = getName(graph_path, 1, 0);
	mod2.load(graph_path + l1mod2path);
	l1himod0.load(graph_path + "hicode/layer1_7.txt");
	string l1himod2path = getName(graph_path, 1, 7);
	l1himod2.load(graph_path + l1himod2path);
	l1himod0sub.load(graph_path + "sub/sub1.gen");

	Communities l2mod0, l2mod2, l2himod0, l2himod2, l2himod0sub, l2himod0sub_reducelayer1;
	l2mod0.load(graph_path + "hicode/layer2_0.txt");
	string l2mod2path = getName(graph_path, 2, 0);
	l2mod2.load(graph_path + l2mod2path);
	l2himod0.load(graph_path + "hicode/layer2_7.txt");
	string l2himod2path = getName(graph_path, 2, 7);
	l2himod2.load(graph_path + l2himod2path);
	l2himod0sub.load(graph_path + "sub/sub2.gen");
	l2himod0sub_reducelayer1.load(graph_path + "sub/sub2_reducelayer1.gen");


	string outdir = "f1/";
	Communities::mkdir(outdir);

	//truth1_1
	Communities * cs11[5];
	cs11[0] = &mod0;
	cs11[1] = &mod2;
	cs11[2] = &l1himod0;
	cs11[3] = &l1himod2;
	cs11[4] = &l1himod0sub;
	char *name11[5] = {"mod0", "mod_leaf", "l1himod0", "l1himod_leaf", "l1himod0sub"};
	fprintf(fp, "truth1_1:%d communities\n", truth1_1.size());
	fprintf(fp, "Detected,NMI,F1,P,R,JF1,JP,JR\n");
	calc(truth1_1, 5, cs11, name11, fp);

	//truth1_2
	fprintf(fp, "truth1_2:%d communities\n", truth1_2.size());
	fprintf(fp, "Detected,NMI,F1,P,R,JF1,JP,JR\n");
	calc(truth1_2, 5, cs11, name11, fp);

	//truth2_1
	Communities * cs21[6];
	cs21[0] = &mod0;
	cs21[1] = &mod2;
	cs21[2] = &l2himod0;
	cs21[3] = &l2himod2;
	cs21[4] = &l2himod0sub;
	cs21[5] = &l2himod0sub_reducelayer1;
	char *name21[6] = { "mod0", "mod_leaf", "l2himod0", "l2himod_leaf", "l2himod0sub","l2himod0sub_reducelayer1" };
	fprintf(fp, "truth2_1:%d communities\n", truth2_1.size());
	fprintf(fp, "Detected,NMI,F1,P,R,JF1,JP,JR\n");
	calc(truth2_1, 6, cs21, name21, fp);

	//truth2_2
	fprintf(fp, "truth2_2:%d communities\n", truth2_2.size());
	fprintf(fp, "Detected,NMI,F1,P,R,JF1,JP,JR\n");
	calc(truth2_2, 6, cs21, name21, fp);



	
	fclose(fp);





	printf("------------\ndone\n");
	return 0;
}