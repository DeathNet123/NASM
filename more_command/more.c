//Have to RUN$ source setting.sh before running the program in order to export lines and columns
#include<stdio.h>
#include <stdlib.h>
#include <time.h>
#include<unistd.h>

//Can't intialize the static variables with non-constant elements
static  int LINES;
static int COLS;
void print_stuff(FILE *printer);
int command_handle(FILE *tty);
int main(int argc, char *argv[])
{
    //Placing the values of LINES AND COLUMNS into a variables..
    LINES = atoi(getenv("LINES"));//since getenv returns char* we converting them into int;
    COLS = atoi(getenv("COLUMNS"));
    //Did this to check weather the I/O was redirected or not -t 0 test weather fd 0 is connect to terminal or not
    if(argc == 1 && !system("test -t 0"))//if stdin is not redirected
    {
        printf("more: bad usage\nTry 'more --help' for more information.\n");
    }
    else if(argc == 1 && system("test -t 0")) //if stdin is redirected
    {
        print_stuff(stdin);
    }
    else //if stdin is not redirected...
    {
        print_stuff(fopen(argv[1], "r"));
    }
    return 0;
}
void print_stuff(FILE *printer)
{
    FILE *command = fopen("/dev/tty", "r");//this is for taking the input from the user...
    char buffer[COLS];
    int idx = 1, choose;
    while(fgets(buffer, COLS, printer) != NULL)
    {
        fputs(buffer, stdout);
        idx++;
        if(idx == LINES-2)
        {
            printf("\e[7m\033[1m --MORE--\e[m");
            choose = command_handle(command);
            printf("\e[1A\033[2K \033[0G");
        }
        switch(choose)
        {
            case 0:
                idx--;
                break;
            default:
                break;
        }
    
    }
    fclose(printer);
}
int command_handle(FILE *tty)
{
    getc(tty);
    return 0;
}