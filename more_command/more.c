//Have to RUN$ source setting.sh before running the program in order to export lines and columns
#include<stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include<unistd.h>
//#define DEBUG
//Can't intialize the static variables with non-constant elements
static  int LINES;
static int COLS;
void handle_editor(FILE *fp);
void print_stuff(FILE *printer, int file_overall);
int count_lines(FILE *f);
int main(int argc, char *argv[])
{ 
    //Non-Canonical Mode input
    system("stty -icanon");
    //system("stty -echo");
    //Placing the values of LINES AND COLUMNS into a variables..
    LINES = atoi(getenv("LINES"));//since getenv returns char* we converting them into int;
    COLS = atoi(getenv("COLUMNS"));
    #ifdef DEBUG
    handle_editor(/*fopen(argv[1], "r")*/stdin);
    #endif
    //Did this to check weather the I/O was redirected or not -t 0 test weather fd 0 is connected to terminal a.k.a tty or not
    if(argc == 1 && !system("test -t 0"))//if stdin is not redirected
    {
        printf("more: bad usage\nTry 'more --help' for more information.\n");
    }
    else if(system("test -t 0")) //if stdin is redirected
    {
        print_stuff(stdin, count_lines(stdin));
    }
    for(int idx = 1; idx < argc; idx++)
    {
        if(argc > 2)
        {
            printf("::::::::::::::\n");
            printf("%s\n", argv[idx]);
            printf("::::::::::::::\n");
        }
        FILE *opener = fopen(argv[idx], "r");
        print_stuff(opener, count_lines(opener));
    }
    system("stty icanon");
    return 0;
}
void print_stuff(FILE *printer, int file_overall)
{
    int lines = 1;
    FILE *command = fopen("/dev/tty", "r");//this is for taking the input from the user...
    char buffer[COLS];
    int idx = 0;
    while(fgets(buffer, COLS, printer) != NULL)
    {
        //printf("LINE: %d: ", lines);
        fputs(buffer, stdout);
        idx++;
        lines++;
        if(idx >= LINES-1)
        {
            printf("\e[7m\033[1m --MORE%0.f%--\e[m", (float) lines / file_overall * 100);
            switch(getc(command))
            {
                case '\n':
                    idx--;
                    break;
                case 'q':
                printf("\n\e[1A\033[2K \033[1G");
                    exit(0);
                case ' ':
                    printf("\n");//this line will add the newline when space bar is pressed so that line 59 can erase the --MORE--
                    idx = 0;
                    break;
                case 'v':
                    handle_editor(printer);
                    printf("\n");
                    break;
                default:
                    break;
            }
            printf("\e[1A\033[2K \033[1G");
        }
    }
    fclose(printer);
}
int count_lines(FILE* f)
{
    int file_overall = 1;
    char buffer[COLS];
    while (fgets(buffer, COLS, f))
    {
        file_overall++;
    }
    rewind(f);
    return file_overall;
}
void handle_editor(FILE *fp)
{
    char filename[COLS];
    char proclink[COLS];
    int real_id = getpid();
    int fd = fileno(fp);
    sprintf(proclink, "/proc/%d/fd/%d", real_id, fd);
    int size = readlink(proclink, filename, COLS);
    filename[size] = '\0';
    int pid = fork();
    if(pid == 0)
    {
        char *editor = "vim";
        char *args[] = {editor, filename, NULL};
        execvp(editor, args);
    }
    wait(NULL);
    return;
}