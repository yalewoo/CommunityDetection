#include <cstdio>
#include <iostream>
#include "../Graph.h"

using std::cout;
using std::endl;

map<string, string> Graph::config;

#include <ctime>

void showVector(vector<int> & v, string name = "vector", int id = 0)
{
	cout << name << " " << id << " (size=" << v.size() << ") : " << endl;
	//for (size_t i = 0; i < v.size(); ++i)
		//cout << v[i] << " ";
	//cout << endl;
}

int main(int argc, char *argv[])
{
	//读配置文件，该文件记录社团检测算法的具体路径
	Graph::loadConfig("F:/Project/CommunityDetection/config.txt");

	string dir = "baseline/";
	Communities::mkdir(dir);

	FILE * ftime = fopen("baseline/time.txt", "a");
	
	time_t start, stop;
	start = time(NULL);
	char tmp[256];
	strftime(tmp, sizeof(tmp), "%Y/%m/%d %X", localtime(&start));
	fprintf(ftime, "-----------------------\n%s\n", tmp);
	//fprintf(ftime, "OutDir: %s\n", argv[2]);
	//fprintf(ftime, "Graph: %s\n", argv[1]);

	fclose(ftime);
	ftime = fopen("baseline/time.txt", "a");

	string graph_path;
	if (argc >= 2)
		graph_path = argv[1];
	else
		graph_path = "F:/HICODE_SUB/syn/3000/";


	bool mod = false, infomap = false;
	bool demon = false, lc = false;
	bool oslom = false, gce = false, cfinder = false;
	for (int i = 2; i < argc; ++i)
	{
		string algname = argv[i];
		if (algname == "mod")
			mod = true;
		if (algname == "infomap")
			infomap = true;
		if (algname == "lc")
			lc = true;
		if (algname == "oslom")
			oslom = true;
		if (algname == "gce")
			gce = true;
		if (algname == "demon")
			demon = true;
		if (algname == "cfinder")
			cfinder = true;
		if (algname == "all")
		{
			mod = true;
			infomap = true;
			lc = true;
			oslom = true;
			gce = true;
			demon = true;
			cfinder = true;

			break;
		}
	}

	Graph g;
	//Graph g;
	//从edge list的文本文件读入图
	//g.load(argv[1]);
	g.load(graph_path + "graph");
	
	fprintf(ftime, "%d nodes, %d edges\n", g.getNodeNum(), g.getEdgeNum());
	fprintf(ftime, "------Time Statistical (second)------\n");


	stop = time(NULL);
	fprintf(ftime, "load Graph: %ld\n", stop - start);


	fclose(ftime);
	

	//string dir = argv[2];
	

	//Mod
	if (mod)
	{
		ftime = fopen("baseline/time.txt", "a");
		start = time(NULL);
		g.runMod().save(dir + "Mod.gen");
		stop = time(NULL);
		fprintf(ftime, "Mod time: %ld\n", stop - start);
		fclose(ftime);
	}

	//Infomap
	if (infomap)
	{
		ftime = fopen("baseline/time.txt", "a");
		start = time(NULL);
		g.runInfomap().save(dir + "Infomap.gen");
		stop = time(NULL);
		fprintf(ftime, "Infomap time: %ld\n", stop - start);
		fclose(ftime);

	}


	//Demon
	if (demon)
	{
		ftime = fopen("baseline/time.txt", "a");
		start = time(NULL);
		g.runDemon().save(dir + "Demon.gen");
		stop = time(NULL);
		fprintf(ftime, "Demon time: %ld\n", stop - start);
		fclose(ftime);
	}
	

	//LC
	if (lc)
	{
		ftime = fopen("baseline/time.txt", "a");
		start = time(NULL);
		g.runLinkComm().save(dir + "LC.gen");
		stop = time(NULL);
		fprintf(ftime, "LC time: %ld\n", stop - start);
		fclose(ftime);
	}



	//return 0;
	//OSLOM
	if (oslom)
	{	
		ftime = fopen("baseline/time.txt", "a");
		start = time(NULL);
		g.runOSLOM2().save(dir + "OSLOM.gen");
		stop = time(NULL);
		fprintf(ftime, "OSLOM time: %ld\n", stop - start);
		fclose(ftime);
	}
	

	//GCE
	if (gce)
	{
		ftime = fopen("baseline/time.txt", "a");
		start = time(NULL);
		g.runGCE().save(dir + "GCE.gen");
		stop = time(NULL);
		fprintf(ftime, "GCE time: %ld\n", stop - start);
		fclose(ftime);
	}




	//CFinder
	if (cfinder)
	{
		ftime = fopen("baseline/time.txt", "a");
		start = time(NULL);
		g.runCFinder().save(dir + "CFinder.gen");
		stop = time(NULL);
		fprintf(ftime, "CFinder time: %ld\n", stop - start);
		fclose(ftime);
	}




	printf("------------\ndone\n");
	return 0;
}