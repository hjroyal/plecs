#include <stdio.h>
#include <stdlib.h>

#include "lib.h"

int main(int argc, char *argv[])
{
	int ret1, ret2;
	int a = 5;
	int b = 2;
	
	ret1 = my_add(a, b);
	ret2 = my_sub(a, b);
	printf("ret1 = %d \n", ret1);
	printf("ret2 = %d \n", ret2);
	getchar();
	return 0;
}