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
    struct dirent **namelist;
    struct stat statbuf;
    LINES = atoi(getenv("LINES"));
    COLS = atoi(getenv("COLUMNS"));

    int count_entries = 0;
    count_entries = scandir(argv[1], &namelist, NULL, alphasort); // it will scandir and place the read structure in my namelist
    /*
    int scandir(const char *dirp, struct dirent ***namelist,
              int (*filter)(const struct dirent *),
              int (*compar)(const struct dirent **, const struct dirent **));
    here the comparison function is alphasort and Filter is pointed towards NULL
    */
    if (count_entries < 0)
        perror("scandir");
    else 
    {
    while (count_entries--) 
        {
            printf("%s\n", namelist[count_entries]->d_name);
            free(namelist[count_entries]);
        }
        free(namelist);
    }
    return 0;
}