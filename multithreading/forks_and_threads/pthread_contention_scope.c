#include<stdio.h>
#include<unistd.h>
#include<pthread.h>

void * runnable_thread(void * arg)
{
    printf("This thread is running in system contention scope\n");
}
int main(void)
{
    pthread_t t1;
    pthread_attr_t attr;
    int scope;
    pthread_attr_init(&attr);
    pthread_attr_getscope(&attr, &scope);
    if(scope == PTHREAD_SCOPE_PROCESS)
        printf("Process SCOPE\n");
    else if(scope == PTHREAD_SCOPE_SYSTEM)
        printf("System SCOPE\n");*/
    pthread_create(&t1, NULL, runnable_thread, NULL);
    pthread_join(t1, NULL);
    pthread_attr_destroy(&attr);
    return 0;
}