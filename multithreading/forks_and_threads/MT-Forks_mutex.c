#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<unistd.h>
#include<sys/wait.h>
#include<sys/types.h>

int count = 0;
pthread_mutex_t m1 = PTHREAD_MUTEX_INITIALIZER;

void * to_do(void *args)
{
    printf("-%d-\n", count);
    pthread_mutex_lock(&m1);
    count++;
    printf("*%d*\n", count);
    int idx = 100000;
    while(idx--);
    pthread_mutex_unlock(&m1);
}
void unlock_mutex(int signum)
{
    pthread_mutex_unlock(&m1);
}
void unlock_mutex_fork(void)
{
    pthread_mutex_unlock(&m1);
}
int main(void)
{
    pthread_atfork(NULL, NULL, unlock_mutex_fork); //(prepare, parent, child)
    signal(SIGINT, unlock_mutex);
    pthread_mutex_init(&m1, NULL);
    pthread_t t1;
    printf("I am parent and i am going to start..\n");
    pthread_create(&t1, NULL, to_do, NULL);
    int cpid = fork();
    if(cpid == 0)
    {
       pthread_t t2;
       printf("I am the child and i am going to start..\n");
       pthread_create(&t2, NULL, to_do, NULL);
       pthread_join(t2, NULL);
    }
    else
    {
        wait(NULL);
    }
    pthread_join(t1, NULL);
    printf("+%d+\n", count);
    pthread_mutex_destroy(&m1);
    return 0;
}