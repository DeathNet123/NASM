//Have to RUN$ source setting.sh before running the program in order to export lines and columns
#include<stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include<string.h>
#include <sys/wait.h>
#include<termios.h>
#include<unistd.h>
//Can't intialize the static variables with non-constant elements
static  int LINES;
static int COLS;
static FILE *command;
static int fd;
static struct termios info;
void handle_editor(FILE *fp);
void print_stuff(FILE *printer, int file_overall);
int count_lines(FILE *f);
int search(int *lines, FILE *fp, int file_overall, FILE * command);
int main(int argc, char *argv[])
{ 
    //Non-Canonical Mode input
    command = fopen("/dev/tty", "r");
    fd = fileno(command);
    tcgetattr(fd, &info);
    info.c_lflag &= ~ICANON;
    tcsetattr(fd, TCSANOW, &info);
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
        char filename[COLS];
        char proclink[COLS];
        int real_id = getpid();//get the pid 
        int fd = fileno(stdin);//get the file descriptor of the opened file.
        sprintf(proclink, "/proc/%d/fd/%d", real_id, fd);//get the link of the opened file from fd table..
        int size = readlink(proclink, filename, COLS);//get the file name using the readlink since they are soft link
        filename[size] = '\0';
        dup2(1, 0); // get back your keyboard on track..
        FILE *fp = fopen(filename, "r");//open the file
        print_stuff(fp, count_lines(fp));//send the stdin pointer to print funtion
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
    info.c_lflag |= ICANON;
    tcsetattr(fd, TCSANOW, &info);
    return 0;
}
void print_stuff(FILE *printer, int file_overall)
{
    int lines = 0;
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
            printf("\e[7m\033[1m --MORE%0.f%%--\e[m", (float) lines / file_overall * 100);
            in_case:
            switch(getc(command))
            {
                case '\n':
                    idx--;
                    break;
                case 'q':
                    printf("\033[2K \033[1G");
                    info.c_lflag |= ICANON;
                    tcsetattr(fd, TCSANOW, &info);
                    exit(0);
                case ' ':
                    printf("\n");//this line will add the newline when space bar is pressed so that line 115 can erase the --MORE--
                    idx = 0;
                    break;
                case 'v':
                    handle_editor(printer);
                    printf("\n");
                    break;
                case '/':
                    printf("\e[2K\e[1G");
                    if(search(&lines, printer, file_overall, command) != -1)
                    {
                        idx = 2;
                        continue;
                    }
                    break;
                case 'n':
                    printf("\n");
                    idx--;
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
    fclose(command);
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
int search(int *lines, FILE *fp, int file_overall, FILE* command)
{
    //Turning on the canonical mode
    int flag = 0;
    int prev = *lines;
    info.c_lflag |= ICANON;
    tcsetattr(fd, TCSANOW, &info);
    printf("/");
    char sub[COLS];
    fgets(sub, COLS, command);
    strtok(sub, "\n");
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
            flag = 1;
            break;
        }
    }
    if(!flag)
    {
        rewind(fp);
        while(prev--)
        {
            fgets(buffer, COLS, fp);
        }
        *lines = prev;
        flag = -1;
    }
    //Turning it back off
    info.c_lflag &= ~ICANON;
    tcsetattr(fd, TCSANOW, &info);
  return flag;
}