#include <cstdio>
#include <iostream>
#include "../Graph.h"

#include "../os.h"
#include "../Config.h"

using std::cout;
using std::endl;

#include <vector>
#include <string>
using std::vector;
using std::string;

extern Config hicode_config;

void showVector(vector<int> & v, string name = "vector", int id = 0)
{
	cout << name << " " << id << " (size=" << v.size() << ") : " << endl;
	//for (size_t i = 0; i < v.size(); ++i)
		//cout << v[i] << " ";
	//cout << endl;
}

bool fileExists(string filename)
{
	bool b;
	FILE * f = fopen(filename.c_str(), "r");
	if (f)
	{
		b = true;
		fclose(f);
	}
	else
		b = false;



	return b;
}

int main_F1(int argc, char *argv[])
{
	string path = "F:/Bio/0420/";
	string detected_path = "F:/Bio/0420/output/hicode_mod_ReduceWeight_2layers_ori_sub/";
	string truth_path = "F:/Bio/0420/";
	string output_path = "F:/Bio/0420/analysis/";
	string baseline_path = "F:/Bio/0420/output/baseline1/";

	Graph g;
	g.load("F:/Bio/0420/output/graph");

	FILE * fp;
	FILE * fp_read;

	//基本情况
	//Detected
	vector<string> d_files;
	for (int i = 1; i <= 2; ++i)
	{
		for (int j = 0; j <= 5; ++j)
		{
			char buff[256];
			string s;
			sprintf(buff, "sub%d_%d.gen", i, j);
			s = buff;
			d_files.push_back(s);
		}
	}
	vector<string> baseline_files;
	baseline_files.push_back("MOD0.gen");
	baseline_files.push_back("MOD1.gen");
	baseline_files.push_back("MOD2.gen");
	baseline_files.push_back("MOD3.gen");
	baseline_files.push_back("Infomap.gen");
	baseline_files.push_back("LC.gen");

	vector<string> t_files;
	for (int i = 0; i <= 14; ++i)
	{
		char buff[256];
		string s;
		sprintf(buff, "level_%d.gen", i);
		s = buff;
		t_files.push_back(s);
	}

	Communities truth_all;
	Communities detected_layer1_all;
	Communities detected_layer2_all;

	string filename;

	//社团基本信息
	filename = output_path + "detected_community.csv";
	if (!fileExists(filename))
	{
		fp = fopen(filename.c_str(), "w");
		fprintf(fp, "detected, #community, average size, Q\n");
		for (int i = 0; i < d_files.size(); ++i)
		{
			string path = detected_path + d_files[i];
			Communities d;
			d.load(path.c_str());
			double Q = g.calcModularity(d);

			fprintf(fp, "%s,%d,%.0lf,%lf\n", d_files[i], d.size(), d.averageSize(), Q);
		}
		fclose(fp);
	}
	else
		printf("%s exists\n", filename.c_str());
	
	filename = output_path + "baseline_community.csv";
	if (!fileExists(filename))
	{
		fp = fopen(filename.c_str(), "w");
		fprintf(fp, "detected, #community, average size, Q\n");
		for (int i = 0; i < baseline_files.size(); ++i)
		{
			string path = baseline_path + baseline_files[i];
			Communities d;
			d.load(path.c_str());
			double Q = g.calcModularity(d);

			fprintf(fp, "%s,%d,%.0lf,%lf\n", baseline_files[i], d.size(), d.averageSize(), Q);
		}
		fclose(fp);
	}
	else
		printf("%s exists\n", filename.c_str());
	
	filename = output_path + "truth_community.csv";
	if (!fileExists(filename))
	{
		fp = fopen(filename.c_str(), "w");
		fprintf(fp, "truth, #community, average size, Q\n");
		for (int i = 0; i < t_files.size(); ++i)
		{
			string path = truth_path + t_files[i];
			Communities t;
			t.load(path.c_str());
			double Q = g.calcModularity(t);

			fprintf(fp, "%s,%d,%.0lf,%lf\n", t_files[i], t.size(), t.averageSize(), Q);
		}
		fclose(fp);
	}
	else
		printf("%s exists\n", filename.c_str());
	

	

	//detected和truth 分层对比
	filename = output_path + "detected_vs_truth_lavel.csv";
	if (!fileExists(filename))
	{
		fp = fopen(filename.c_str(), "w");
		fprintf(fp, "detected, truth, truth average f1, detected average f1, truth weighted f1, detected weighted f1\n");
		for (int i = 0; i < d_files.size(); ++i)
		{
			string path = detected_path + d_files[i];
			Communities d;
			d.load(path.c_str());

			for (int j = 0; j < t_files.size(); ++j)
			{
				string path = truth_path + t_files[j];
				Communities t;
				t.load(path.c_str());

				printf("%d vs %d\n", i, j);

				double t_f1, d_f1, t_wf1, d_wf1;
				t_f1 = Communities::f1(t, d);
				d_f1 = Communities::f1(d, t);
				t_wf1 = Communities::wf1(t, d);
				d_wf1 = Communities::wf1(d, t);
				fprintf(fp, "%s,%s,%lf,%lf,%lf,%lf\n", d_files[i], t_files[j], t_f1, d_f1, t_wf1, d_wf1);
			}
		}
		fclose(fp);
	}
	else
		printf("%s exists\n", filename.c_str());
	

	//baseline对比truth 分层对比
	filename = output_path + "baseline_vs_truth_level.csv";
	if (!fileExists(filename))
	{
		fp = fopen(filename.c_str(), "w");
		fprintf(fp, "detected, truth, truth average f1, detected average f1, truth weighted f1, detected weighted f1\n");
		for (int i = 0; i < baseline_files.size(); ++i)
		{
			string path = baseline_path + baseline_files[i];
			Communities d;
			d.load(path.c_str());

			for (int j = 0; j < t_files.size(); ++j)
			{
				string path = truth_path + t_files[j];
				Communities t;
				t.load(path.c_str());

				printf("%d vs %d\n", i, j);

				double t_f1, d_f1, t_wf1, d_wf1;
				t_f1 = Communities::f1(t, d);
				d_f1 = Communities::f1(d, t);
				t_wf1 = Communities::wf1(t, d);
				d_wf1 = Communities::wf1(d, t);
				fprintf(fp, "%s,%s,%lf,%lf,%lf,%lf\n", baseline_files[i], t_files[j], t_f1, d_f1, t_wf1, d_wf1);
			}
		}
		fclose(fp);
	}
	else
		printf("%s exists\n", filename.c_str());


	//truth 和 detected 的每个社团对比
	Communities detected;
	detected.load("F:/Bio/0420/output/hicode_mod_ReduceWeight_2layers_ori_sub/all.gen");
	detected.calcModularity(g);

	filename = output_path + "community_truth_detected.csv";
	if (!fileExists(filename))
	{
		fp = fopen(filename.c_str(), "w");
		fprintf(fp, "level, #,size,inter edges, outer edges, modularity\n");
		for (int i = 0; i < t_files.size(); ++i)
		{
			string path = truth_path + t_files[i];
			Communities t;
			t.load(path.c_str());
			t.calcModularity(g);

			for (int j = 0; j < t.size(); ++j)
			{
				printf("%d\n", j);
				fprintf(fp, "%d,%d,%d,%lf,%lf,%lf\n", i, j, t.comms[j].size(), 
					t.comm_inter_edge_num[j], t.comm_out_edge_num[j],t.getCommQ(j));
			}
		}
		fclose(fp);
		
		
	}
	else
		printf("%s exists\n", filename.c_str());



	return 0;
}