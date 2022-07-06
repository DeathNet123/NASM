#include<stdio.h>
#include<unistd.h>

void getinput()
{
    char buf[10];
    int rv = read(0, buf, 1000);
    printf("%d", rv);
}
int main(void)
{
    getinput();
    return 0;
}
