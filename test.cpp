#include <cstdio>

int linux_rand()
{
	static FILE * fp = fopen("tools/linux_rand_array.txt", "r");
	int n;
	if (fscanf(fp, "%d", &n) == 1)
		return n;
	else
		return 1;
}

int main()
{
	for (int i = 0; i < 20; ++i)
		printf("%d\n", linux_rand());


	return 0;
}