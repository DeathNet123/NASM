#include<stdio.h>
#include <stdlib.h>
#include<unistd.h>
#include<readline/readline.h>
#include<readline/history.h>


int main(void)
{
    setenv("INPUTRC", "inputrc", 0);
    while(1)
    {
        rl_attempted_completion_function = custom_command_completion;
        char *argv[3];
        argv[0] = "ls";
        argv[1] = "-l";
        argv[2] = NULL;
        char *buffer;
        buffer = readline(">");
        add_history(buffer);
        
        free(buffer);
        
    }
    return 0;
}
char **custom_command_completion(const char *text, int start, int end)
{
    if(command_flag)
    {
        text[start];
        char ad[50];
        sprintf(ad, "/usr/bin/%s", text);
        char **arry = rl_completion_matches(ad, rl_filename_completion_function);
        for(int idx = 0; arry[idx] != NULL; idx++) 
        {
            char *new = (char *)malloc(sizeof(char) * 100);
            sprintf(new,"%s\0", arry[idx] + 9);
            free(arry[idx]);
            arry[idx] = new;
        }
        command_flag = 0;
        return arry;
    }
    command_flag = 1;
    return rl_completion_matches(text, rl_filename_completion_function);
}