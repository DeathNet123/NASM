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

void clean_command(char *command);
int main(int argc, char **argv)
{
    char buff[1024];
    while(1)
    {
        fgets(buff, sizeof(buff), stdin);
        clean_command(buff);
        printf("%s\n", strtok(buff, "|"));
        while(1)
        {
                printf("%s\n", strtok(NULL, "|"));
        }
        memset(buff, 0, sizeof buff);
    }
}

void clean_command(char *command)
{
    char cmd[1024];
    memset(cmd, '\0', 1024);
    int count = 0;
    strcpy(cmd, command);
    printf("%s\n", cmd);
    memset(command, '\0', 1024);
    for(int idx = 0; idx < 1024; idx++)
    {
        if((cmd[idx] >= 'a' && cmd[idx] <= 'z') || (cmd[idx] >= 'A' && cmd[idx] == 'Z') || (cmd[idx] >= '0' && cmd[idx] <= '9') || cmd[idx] == '-' || cmd[idx] == '&' || cmd[idx] == '|') 
        {
            command[count++] = cmd[idx];
        }
    }
}