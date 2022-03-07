#include <stdio.h>
#include<stdlib.h>
#include<unistd.h>
int main(void)
{
    int cake;
    int cpid = fork();
    if(cpid == 0)
    {
        while(1)
        printf("%d\n", (int)getppid());
    }
    else
    {
        printf("%d\n", (int)getpid());
        scanf("%d", &cake);
    }
        
    
    return 0;
}