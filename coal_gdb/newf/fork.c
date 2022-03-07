#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
int main(int argc, char *argv[])
{ 
    fork() && fork() && fork();
    fprintf(stdout, "PUCIT\n");
    exit(0);
}
