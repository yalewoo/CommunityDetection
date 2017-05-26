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


	FILE * ftime = fopen("time.txt", "a");
	
	time_t start, stop;
	start = time(NULL);
	char tmp[256];
	strftime(tmp, sizeof(tmp), "%Y/%m/%d %X", localtime(&start));
	fprintf(ftime, "-----------------------\n%s\n", tmp);
	fprintf(ftime, "OutDir: %s\n", argv[2]);
	fprintf(ftime, "Graph: %s\n", argv[1]);

	fclose(ftime);
	ftime = fopen("time.txt", "a");

	Graph g;
	//Graph g;
	//从edge list的文本文件读入图
	g.load(argv[1]);
	fprintf(ftime, "%d nodes, %d edges\n", g.getNodeNum(), g.getEdgeNum());
	fprintf(ftime, "------Time Statistical (second)------\n");


	stop = time(NULL);
	fprintf(ftime, "load Graph: %ld\n", stop - start);


	fclose(ftime);
	ftime = fopen("time.txt", "a");

	string dir = argv[2];
	Communities::mkdir(dir);

	//Infomap
	start = time(NULL);
	g.runInfomap().save(dir + "Infomap.gen");
	stop = time(NULL);
	fprintf(ftime, "Infomap time: %ld\n", stop - start);
	fclose(ftime);
	ftime = fopen("time.txt", "a");

	//Mod
	start = time(NULL);
	g.runMod().save(dir + "Mod.gen");
	stop = time(NULL);
	fprintf(ftime, "Mod time: %ld\n", stop - start);
	fclose(ftime);
	ftime = fopen("time.txt", "a");

	//Demon
	start = time(NULL);
	g.runDemon().save(dir + "Demon.gen");
	stop = time(NULL);
	fprintf(ftime, "Demon time: %ld\n", stop - start);
	fclose(ftime);
	ftime = fopen("time.txt", "a");

	//LC
	start = time(NULL);
	g.runLinkComm().save(dir + "LC.gen");
	stop = time(NULL);
	fprintf(ftime, "LC time: %ld\n", stop - start);
	fclose(ftime);
	ftime = fopen("time.txt", "a");

	//OSLOM
	start = time(NULL);
	g.runOSLOM2().save(dir + "OSLOM.gen");
	stop = time(NULL);
	fprintf(ftime, "OSLOM time: %ld\n", stop - start);
	fclose(ftime);
	ftime = fopen("time.txt", "a");

	//GCE
	start = time(NULL);
	g.runGCE().save(dir + "GCE.gen");
	stop = time(NULL);
	fprintf(ftime, "GCE time: %ld\n", stop - start);
	fclose(ftime);
	ftime = fopen("time.txt", "a");



	//CFinder
	//start = time(NULL);
	// g.runCFinder().save(dir + "CFinder.gen");
	// stop = time(NULL);
	// fprintf(ftime, "CFinder time: %ld\n", stop - start);
	// fclose(ftime);
	// ftime = fopen("time.txt", "a");

	

	fclose(ftime);



	printf("------------\ndone\n");
	return 0;
}