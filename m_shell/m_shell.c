#include<stdio.h>
#include<signal.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#include<fcntl.h>
#include <sys/types.h>
#include<constant.h> //this is my header
#include<m_shell_struct.h>
#include <sys/wait.h>
#include<regex.h>
#include<unistd.h>
#include<wait.h>

extern char **environ;
int logical_command = 0;
int pipe_command = 0;

#define DEV

int handle_logical_command(char *command, regex_t *control_preg);//this will handle the logical Operators in the given in put
/*Handle control is the main entry point and shell will always check for it */
int spawn_child(char *file, char **argv); //this function will called when the child process has to be created and command is external one.
internal_command type_a(char * command); //this function will test weather the command is internal or external 
int handle_internal_command(char *command, internal_command meta_data, char **argv); //this function will called when the internal command is encountered
void clear_command(char *command);
int handle_command_pipes(char *command);

int main(int argc, char **argv)
{
    //compile the control command regex
    regex_t control_preg;
    regcomp(&control_preg, "(.+)(&&|[|][|])(.+)", REG_EXTENDED);
    
    handle_command_pipes("clear"); //this will cause the shell to clear the screen..
    char command[MAX_CMD_LEN];//buffer for holding the command
    #ifdef DEBUG
    for(int idx = 0; environ[idx] != NULL; idx++)
    {
        printf("%s\n", environ[idx]);
    }
    #endif
    while(1)
    {
        memset(command, '\0', MAX_CMD_LEN);
        #ifndef DEV
        printf("(%s@%s)-[%s]-$ ", getenv("LOGNAME"), getenv("NAME"), strcmp(getenv("HOME"), getenv("HOME"))?"~":getenv("PWD"));  
        #endif
        #ifdef DEV
        printf("\n(%s@%s)-$ ", getenv("LOGNAME"), getenv("NAME"));
        #endif
        fgets(command, MAX_CMD_LEN, stdin);
        printf("%s", command);
        handle_logical_command(strtok(command, "\n"), &control_preg); //checking for the control commands if exist...
        if(logical_command)
        {
            logical_command = 0;
            continue;
        }
        handle_command_pipes(command);
    }
	return 0;
}

int handle_logical_command(char *command, regex_t *control_preg)
{
    regmatch_t pmatch[CONTROL_COMMAND_GROUP];
    int test = regexec(control_preg, command, CONTROL_COMMAND_GROUP, pmatch, 0);
    if(test) //check if command contains logical Operators or not if not return immediately..
    {
        return 0;
    } 
    logical_command = 1;

    //extracting the segments of the control command
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

    //extracting the operator
    int op = 0;
    if(strcmp(operator, "&&") == 0) op = 1;
    else op = 2;
    
    int deep_well = regexec(control_preg, left, CONTROL_COMMAND_GROUP, pmatch, 0); //checking weather we have to break down further or not...
    int last_status = handle_logical_command(left, control_preg);
    
    if(deep_well != 0)//if we don't have to break the command further will stop and handle the both left and right side and return..
    {
        switch(op) 
        {
            case 1:
                {
                    int right_status = 0;
                    int status = handle_command_pipes(left);  
                    if(WIFEXITED(status) && status >> 8 != COM_NOT_EXIST) //if the left child exited normally then go fo rthe right thus satisfying the && logical Operator..
                    {
                        right_status = handle_command_pipes(right);
                        if(WIFEXITED(right_status))
                            return 1;
                        else return 0;
                    }
                    else return 0;
                }
                break;
            case 2:
                int left_status = handle_command_pipes(left);
                int right_status = handle_command_pipes(right);
                return WIFEXITED(left_status) || WIFEXITED(right_status);
                break;
            default:
                printf("\nwe are broken somewhat!\n");
                exit(SABNORMAL); //right now this is explicit later will be replaced with the define one..
                break;
        }
    }
    else //this will occur once all of the stacks will be getting exhausted in reverse order..
    {
        switch (op)
        {
            case 1:
                if(last_status == 1)
                {
                    int status = handle_command_pipes(right);
                    if(WIFEXITED(status))
                        return 1;
                    else return 0;
                } 
                else return 0;
                break;
            case 2:
                int status = handle_command_pipes(right);
                return last_status && WIFEXITED(status);
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

void clean_command(char *command) // the purpose of this function is to remove the characters such as \n and \r etc and extra white spaces
{
    char cmd[1024];
    memset(cmd, '\0', 1024);
    int count = 0;
    strcpy(cmd, command);
    printf("%s\n", cmd);
    memset(command, '\0', 1024);
    char prev = cmd[0];
    for(int idx = 0; idx < 1024; idx++)
    {
        if(cmd[idx] == '~'||cmd[idx] == ' ' ||(cmd[idx] >= 'a' && cmd[idx] <= 'z') || (cmd[idx] >= 'A' && cmd[idx] == 'Z') || (cmd[idx] >= '0' && cmd[idx] <= '9') || cmd[idx] == '-' || cmd[idx] == '&' || cmd[idx] == '|') 
        {
            if(prev != ' ' && cmd[idx] != ' ')
                command[count++] = cmd[idx];
            prev = cmd[idx];
        }
    }
}

int handle_command_pipes(char *command)
{
    char *argv[ARG_NUM];
    argv[0] = command;
    argv[1] = NULL;
    int rv = spawn_child(command, argv);
    return rv;
}