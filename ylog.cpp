#ifndef Y_DEBUG_H
#define Y_DEBUG_H


#include <cstdio>


//#define Y_DEBUG

void ylog(char * s)
{
#ifdef Y_DEBUG
	printf("%s\n", s);
#endif
}

void ylog(int n)
{
#ifdef Y_DEBUG
	printf("%d\n", n);
#endif
}


#endif