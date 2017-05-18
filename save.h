#include <iostream>
#include <vector>
#include <stack>
#include <queue>
#include <string>
#include <algorithm>

using namespace std;

#include <cstdio>
#include <cstdlib>
#include <cstring>



void saveVector(vector<double> & v, const char * fn)
{
	FILE *fp = fopen(fn, "w");
	for (int i = 0; i < v.size(); ++i)
	{
		fprintf(fp, "%lf\n", v[i]);
	}
	fclose(fp);
}