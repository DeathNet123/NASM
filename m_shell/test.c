#include<stdio.h>
#include<signal.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#include<fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include<regex.h>
#include<unistd.h>
#include<wait.h>

int main(int argc, char **argv)
{
    printf("%d", 0xFFFF);
    if(WIFEXITED(0xFFFF))
    {
        printf("I WAS HERE");
    }
}