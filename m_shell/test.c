#include<stdio.h>
#include <stdlib.h>
#include<unistd.h>
#include<wait.h>

int main(int argc, char **argv)
{
    execlp("ls", "myls", NULL);
}