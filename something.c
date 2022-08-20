#include<stdio.h>

int add(int a, int b)
{
	return a+b;
}
int main(void)
{
	int (*ptr)(int, int) = add
	int *(ptr1)(int, int) = add
	printf("%d\n", ptr(1,2));
	printf("%d\n", ptr1(1,2));
	return 0;
}
