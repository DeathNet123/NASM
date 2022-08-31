#include<stdio.h>
#include <stdlib.h>
#include<unistd.h>
#include<wait.h>
#include<readline/readline.h>
#include<readline/history.h>

void reaper(int signal)
{
    waitpid(-1, NULL, WCONTINUED);
}
int main(void)
{
    signal(SIGCHLD, reaper);
    int cp = fork();
    if(cp == 0)
    {
        char *argv[3];
        argv[0] = "sleep";
        argv[1] = "10";
        argv[2] = NULL;
        execvp(argv[0], argv);
    }
    else
    {
        char *buffer;
        kill(20, cp);
        kill(18, cp);
        while(1)
        {
            buffer = readline(">");
            free(buffer);
        }
    }

    for(int idx = 0; idx < 3)
    return 0;
}