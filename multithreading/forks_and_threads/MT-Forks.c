#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<unistd.h>
#include<sys/wait.h>
#include<sys/types.h>
int cpid = -1;
void * to_do(void *args)
{
    
    sleep(100);
    int idx = 10;
    while(idx--)
    {
        printf("my cpid is %d ", cpid);
        printf("%d\n", idx);
    }   
}
int main(void)
{
    pthread_t t1;
    pthread_create(&t1, NULL, to_do, NULL);
    cpid = fork();
    if(cpid == 0)
    {
        printf("i am here in the Child process and i have no thread ??");
        pause();
    }
    if(cpid != 0)
        wait(NULL);
    pthread_join(t1, NULL);
    perror("pthread_join");
    exit(0);
}