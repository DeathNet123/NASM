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

extern char **environ;
#define MAX_CMD_LEN 4096 // The max lenght of the command at one time. this is same as bash.....
#define CONTROL_COMMAND_GROUP 4 //Groups count for the control command regex
#define ARG_NUM 50
#define ARG_LEN 25
#define DEV
bool handle_control_command(char *command, regex_t *control_preg);

int main(int argc, char **argv)
{
    //compile the control command regex
    regex_t control_preg;
    regcomp(&control_preg, "(.+)(&&|[|][|])(.+)", REG_EXTENDED);
    
    char command[MAX_CMD_LEN];
    #ifdef DEBUG
    for(int idx = 0; environ[idx] != NULL; idx++)
    {
        printf("%s\n", environ[idx]);
    }
    #endif
    while(true)
    {
        #ifndef DEV
        printf("(%s@%s)-[%s]-$ ", getenv("LOGNAME"), getenv("NAME"), strcmp(getenv("HOME"), getenv("HOME"))?"~":getenv("PWD"));  
        #endif
        #ifdef DEV
        printf("\n(%s@%s)-$ ", getenv("LOGNAME"), getenv("NAME"));
        #endif
        fgets(command, MAX_CMD_LEN, stdin);
        handle_control_command(command, &control_preg); //calling 
    }
	return 0;
}

bool handle_control_command(char *command, regex_t *control_preg)
{
    regmatch_t pmatch[CONTROL_COMMAND_GROUP];
    int test = regexec(control_preg, command, CONTROL_COMMAND_GROUP, pmatch, 0);
    
    if(test) //check if command contains logical Operators or not..
    {
        return false;
    }

    //extracting the peices of the control command
    char left[pmatch[1].rm_eo - pmatch[1].rm_so];
    char operator[3];
    char right[pmatch[3].rm_eo - pmatch[3].rm_so];
    memset(left, '\0', sizeof(left));
    memset(right, '\0', sizeof(right));
    memset(operator, '\0', sizeof(operator));
    strncpy(left, command + pmatch[1].rm_so, pmatch[1].rm_eo - pmatch[1].rm_so);
    strncpy(operator, command + pmatch[2].rm_so, pmatch[2].rm_eo - pmatch[2].rm_so);
    strncpy(right, command + pmatch[3].rm_so, pmatch[3].rm_eo - pmatch[3].rm_so);
    #ifdef DEV
    printf("LEFT:%s OP:%s RIGHT:%s", left, operator, right);
    #endif
    int op = 1;
    switch(op) 
    {
        case 1:
            char *arg_left[ARG_NUM];
            arg_left[0] = left;
            arg_left[1] = NULL; 
            int left_fork = fork();
            if(left_fork == 0)
            {
                int rv = execvp(left, arg_left);
                perror("execv");
                printf("%d", rv);
            }
            else
            {
                int status;
                wait(&status);
            
                if(WIFEXITED(status))
                {
                    char *arg_right[ARG_NUM];
                    arg_right[0] = right;
                    arg_right[1] = NULL; 
                    int right_fork = fork();
                    if(right_fork == 0)
                    {
                        execvp(right, arg_right);
                    }
                    else
                    {
                        wait(NULL);
                    }
                }
            }
            break;
        case 2:
            printf("||");
            break;
        default:
            printf("we are broke somewhat!");
            break;
    }
}