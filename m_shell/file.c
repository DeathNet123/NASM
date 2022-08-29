#include<stdio.h>
#include <stdlib.h>
#include<unistd.h>
#include<wait.h>
#include<readline/readline.h>
#include<readline/history.h>

int main(void)
{
    while(1)
    {
       char *buffer;
        buffer = readline(">");
        add_history(buffer);
        if(buffer[0] == '$');
        char *value = getenv(buffer + 1);
        if(value != NULL)
            printf("%s\n", value);
        free(buffer);
        printf("%d", strlen("12"));   
    }
    return 0;
}