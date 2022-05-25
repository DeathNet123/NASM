#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h> 
#include <sys/types.h>
#include <sys/wait.h>

void *t_run(void *arg)
{
    int ca = fork();
    if(ca != 0)
    {
        printf("i am inside the parent process and this is my child %d PID\n", ca); 
        wait(NULL); 
        printf("i am parent here and this is my PID %ld\n", (long)getpid()); 
    }
    else
        printf("I am the child process and this is my %ld\n", (long)getpid());
}
int main(void)
{
    pthread_t t1;
    pthread_create(&t1, NULL, t_run, NULL);
    pthread_join(t1, NULL);
    printf("This is my PID %ld\n", (long)getpid()); 
    return 0;
}

/*
OutPut of this program will be
i am inside the parent process and this is my child 226 PID
I am the child process and this is my 226
i am parent here and this is my PID 224
This is my PID 224

Fork man Page..
The child process is created with a single thread—the one that called fork().  The entire  vir‐tual  
address  space of the parent is replicated in the child, including the states of mutexes,condition variables, 
and other pthreads objects; the use of pthread_atfork(3)  may  be  helpful for dealing with problems that this can cause.
*/