#include <cstdio>
#include <iostream>
#include "../Graph.h"
#include "../Config.h"


using std::cout;
using std::endl;

map<string, string> Graph::config;

template <typename T>
void showVector(vector<T> & v, string name = "vector", int id = 0)
{
	cout << name << " " << id << " (size=" << v.size() << ") : " << endl;
	for (size_t i = 0; i < v.size(); ++i)
		cout << v[i] << "\n";
	cout << endl;
}



#include "../os.h"

int main(int argc, char *argv[])
{
	Graph::loadConfig("F:/Project/CommunityDetection/config.txt");

	Config con;
	con.updateConfig("F:/HICODE_SUB/syn/testGraph/hicode.config");

	os::moveDir("apple", "F:/");
	

	printf("------------\ndone\n");
	return 0;
}