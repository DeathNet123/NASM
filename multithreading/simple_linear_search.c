#include<stdio.h>
#include <stdlib.h>
#include<pthread.h>
#include<unistd.h>

int flag = 0;
int data[13] = {5, 6, 8, 16, 19, 23, 29, 31, 15, 17, 41, 43, 47};

int main(void)
{
    struct arguments args1, args2;
    args1.s = 0;
    args2.e = 5;
    args2.s = 5;
    args2.e = 10;
    args1.key = 47;
    args2.key = 47;
    pthread_t tid1, tid2;
    pthread_create(&tid1, NULL, linear, (void *) & args1);
    pthread_create(&tid1, NULL, linear, (void *) & args2);
    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);
    if(flag)
        printf("Yes");
    else
        printf("No");
    return 0;
}