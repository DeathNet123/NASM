#include<stdio.h>
#include<signal.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#include<fcntl.h>
#include <sys/types.h>
#include<constant.h> //this is my header
#include <sys/wait.h>
#include<regex.h>
#include<unistd.h>
#include<wait.h>

extern char **environ;

#define DEV

bool handle_control_command(char *command, regex_t *control_preg);//this will handle the logical Operators in the given in put
/*Handle control is the main entry point and shell will always check for it */
int spawn_child(char *file, char **argv); //this function will called when the child process has to be created and command is external one.
bool type_a(char * file); //this function will test weather the command is internal or external it is bool right now later will return struct..
int handle_internal_command(char *file /*, struct later will be added as function pointer*/);//this function will called when the internal command is encountered
void clean_command(char *command);

int main(int argc, char **argv)
{
    //compile the control command regex
    regex_t control_preg;
    regcomp(&control_preg, "(.+)(&&|[|][|])(.+)", REG_EXTENDED);
    
    char command[MAX_CMD_LEN];//buffer for holding the command
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
        clean_command(command);
        printf("%s", command);
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
    char left[pmatch[1].rm_eo - pmatch[1].rm_so + 1];
    char operator[3];
    char right[pmatch[3].rm_eo - pmatch[3].rm_so + 1];
    memset(left, '\0', sizeof(left));
    memset(right, '\0', sizeof(right));
    memset(operator, '\0', sizeof(operator));
    strncpy(left, command + pmatch[1].rm_so, pmatch[1].rm_eo - pmatch[1].rm_so);
    strncpy(operator, command + pmatch[2].rm_so, pmatch[2].rm_eo - pmatch[2].rm_so);
    strncpy(right, command + pmatch[3].rm_so, pmatch[3].rm_eo - pmatch[3].rm_so);
    #ifdef DEV
    printf("\nLEFT:%s OP:%s RIGHT:%s\n", left, operator, right);
    #endif
    int op = 0;
    if(strcmp(operator, "&&") == 0) op = 1;
    else op = 2;
    char *argv[ARG_NUM];
    int deep_well = regexec(control_preg, left, CONTROL_COMMAND_GROUP, pmatch, 0);
    int last_status = handle_control_command(left, control_preg);
    if(deep_well != 0)
    {
        switch(op) 
        {
            case 1:
                {
                    int right_status = 0;
                    int status = spawn_child(left, argv);  
                    if(WIFEXITED(status) && status >> 8 != COM_NOT_EXIST) //if the left child exited normally then go fo rthe right thus satisfying the && logical Operator..
                    {
                        right_status = spawn_child(right, argv);
                        if(WIFEXITED(right_status))
                            return true;
                        else return false;
                    }
                    else return false;
                }
                break;
            case 2:
                int left_status = spawn_child(left, argv);
                int right_status = spawn_child(right, argv);
                return WIFEXITED(left_status) || WIFEXITED(right_status);
                break;
            default:
                printf("\nwe are broken somewhat!\n");
                exit(SABNORMAL); //right now this is explicit later will be replaced with the define one..
                break;
        }
    }
    else
    {
        switch (op)
        {
            case 1:
                if(last_status == true)
                {
                    int status = spawn_child(right, argv);
                    if(WIFEXITED(status))
                        return true;
                    else return false;
                } 
                else return false;
                break;
            case 2:
                int status = spawn_child(right, argv);
                return deep_well && WIFEXITED(status);
                break;
            default:
                printf("\nTHERE WAS A PROBLEM IN RECURSION\n");
                exit(SABNORMAL);
                break;

        }
    }
}

int spawn_child(char *file, char **argv)
{
    argv[0] = file;
    argv[1] = NULL; 
    /*the arguments will be filled by argument handler function later..*/
    int cpid = fork();
    if(cpid == 0)
    {
        int rv = execvp(file, argv);
        if(rv != 0)
        {
            printf("No such command found: %s\n", file);
            return COMMAND_NOT_FOUND;
        }
    }
    else
    {
        int status = 0;
        wait(&status);
        return status;
    }
}

void clean_command(char *command)
{
    char cmd[MAX_CMD_LEN];
    for(int idx = 0; idx < strlen(command); idx++)
    {
        if((command[idx] >= '0' && command[idx] <= '9') || (command[idx] >= 'a' && command[idx] <= 'z') || command[idx] == '&' || command[idx] == '|' || command[idx] == '>' || command[idx] == '<' || (command[idx] >= 'A' && command[idx] <= 'Z'))
        {
            cmd[idx] = command[idx];
        }
    }
    strcpy(command, cmd);
    command[strlen(command)] = '\0';
}