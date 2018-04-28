#include <map>
#include "Graph.h"
#include "Config.h"

map<string, string> Graph::config;

int main_hicode(int argc, char *argv[]);
int main_F1(int argc, char *argv[]);


Config hicode_config;

int main(int argc, char *argv[])
{
	Graph::loadConfig(ALG_CONFIG_PATH);

	
	hicode_config.updateConfig("F:/Project/CommunityDetection/hicode_default.config");






	//return main_hicode(argc, argv);
	//return main_F1(argc, argv);

	Graph g;
	g.load("F:/HICODE_SUB/0426new/graph");
	Communities t11, t12;
	t11.load("F:/HICODE_SUB/0426new/L11.txt");
	t12.load("F:/HICODE_SUB/0426new/L12.txt");
	Communities & comm1 = t11;
	Communities * comm2 = &t12;


	double nmi = comm1.calcNMI(*comm2);
	printf("NMI = %lf\n", nmi);


	double f1_unweighted = Communities::F1_unweighted(comm1, *comm2);
	double f1_weighted = Communities::F1_weighted(*comm2, comm1);

	double jaccard_f1_unweighted = Communities::Jaccard_F1_unweighted(comm1, *comm2);
	double jaccard_f1_weighted = Communities::Jaccard_F1_weighted(*comm2, comm1);

	double f1_precision_unweighted = Communities::f1(comm1, *comm2);
	double f1_recall_unweighted = Communities::f1(*comm2, comm1);

	double f1_precision_weighted = Communities::wf1(comm1, *comm2);
	double f1_recall_weighted = Communities::wf1(*comm2, comm1);

	printf("F1 = %lf, wF1 = %lf\n", f1_unweighted, f1_weighted);

	printf("JF1 = %lf, wJF1 = %lf\n", jaccard_f1_unweighted, jaccard_f1_weighted);

	printf("F1_precision = %lf, wF1_precision = %lf\n", f1_precision_unweighted, f1_recall_unweighted);

	printf("F1_recall = %lf, wF1_recall = %lf\n", f1_precision_weighted, f1_recall_weighted);

}