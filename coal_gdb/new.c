#include<stdio.h>
#include<stdlib.h>
int idx = 1;
int main(int argc, char *argv[])
{
	if(argc < 2) return -1;
	int temp = atoi(argv[1]);
	int fact = 1;
	for(; idx <= temp; idx++)
		fact *= idx;
	printf("Factorial of %d is %d\n", temp, fact);
	return 0;
}
