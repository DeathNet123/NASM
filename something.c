#include<stdio.h>

int add(int a, int b)
{
	return a+b;
}
int main(void)
{
	int list[5];
	list[0] = add(1, 2);
	return 0;
}
