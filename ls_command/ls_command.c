//Have to Run$ source setting.sh in order to export the lines and columns
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include <sys/types.h>
#include<dirent.h>
#include <sys/stat.h>
#include <time.h>

static int LINES;
static int COLS;

int main(int argc, char *argv[])
{
    struct dirent *entry;
    struct stat statbuf;
    LINES = atoi(getenv("LINES"));
    COLS = atoi(getenv("COLUMNS"));

    struct dirent **namelist;
    int n;
    n = scandir(argv[1], &namelist, NULL, alphasort);
    if (n < 0)
        perror("scandir");
    else 
    {
    while (n--) 
        {
            printf("%s\n", namelist[n]->d_name);
            free(namelist[n]);
        }
        free(namelist);
    }
    return 0;
}