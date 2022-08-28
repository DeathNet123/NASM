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
        buffer[5] = '\0';
        while(1)
        {
            char *meance = strsep(&buffer, " ");
            if(meance == NULL) break;
            printf("%s\n", meance);
        } 
        free(buffer);
        
    }
    return 0;
}