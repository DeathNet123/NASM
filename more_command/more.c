//Have to RUN$ source setting.sh before running the program in order to export lines and columns
#include<stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include<string.h>
#include <sys/wait.h>
#include<unistd.h>
//Can't intialize the static variables with non-constant elements
static  int LINES;
static int COLS;
void handle_editor(FILE *fp);
void print_stuff(FILE *printer, int file_overall);
int count_lines(FILE *f);
void search(int *lines, FILE *fp, int file_overall);
int main(int argc, char *argv[])
{ 
    //Non-Canonical Mode input
    system("stty -icanon");
    //system("stty -echo");
    //Placing the values of LINES AND COLUMNS into a variables..
    LINES = atoi(getenv("LINES"));//since getenv returns char* we converting them into int;
    COLS = atoi(getenv("COLUMNS"));
    //Did this to check weather the I/O was redirected or not -t 0 test weather fd 0 is connected to terminal a.k.a tty or not
    if(argc == 1 && !system("test -t 0"))//if stdin is not redirected
    {
        printf("more: bad usage\nTry 'more --help' for more information.\n");
    }
    else if(system("test -t 0")) //if stdin is redirected
    {
        print_stuff(stdin, count_lines(stdin));//send the stdin pointer to print funtion
    }
    //This will print the remaining arguments file.......
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
    int lines = 0;
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
            in_case:
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
                case '/':
                    printf("\e[2K\e[1G");
                    search(&lines, printer, file_overall);
                    idx = 2;
                    continue;
                    break;
                default:
                    printf("\e[1D\e[0K");
                    goto in_case;
                    break;
            }
            printf("\e[1A\033[2K \033[1G");
        }
    }
    fclose(printer);
}
int count_lines(FILE* f)//This function will count the lines and return the number of lines in file and it makes sure to rewind the pointer to start..
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
    int real_id = getpid();//get the pid 
    int fd = fileno(fp);//get the file descriptor of the opened file.
    sprintf(proclink, "/proc/%d/fd/%d", real_id, fd);//get the link of the opened file from fd table..
    int size = readlink(proclink, filename, COLS);//get the file name using the readlink since they are soft link
    filename[size] = '\0';//set the filename last character as \0 the NOTE: readlink will place the number of bytes it will read Nice right smh and I love Minha :)
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
void search(int *lines, FILE *fp, int file_overall)
{
    system("stty icanon"); //turning on canonical mode
    printf("/");
    char sub[COLS];
    scanf("%s", &sub);
    char buffer[COLS];
    unsigned int skipper = 0;
    while((*lines) <= file_overall)
    {
        fgets(buffer, COLS, fp);
        (*lines)++;
        skipper++;
        if(strstr(buffer, sub) != NULL)
        {
            if(skipper > 2)
                printf("skipping...\n");
            fputs(buffer, stdout);
            break;
        }
    }
    system("stty -icanon");//turning off canonical mode
    return;
}