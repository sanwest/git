#include <stdio.h>

int main(int a,char **b)
{
	if(a>=2)
		printf("Hello,%s!\n",b[1]);
	else
		printf("hello world!\n");
	return 0;
}

