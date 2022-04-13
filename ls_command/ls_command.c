//Have to Run$ source setting.sh in order to export the lines and columns
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include <sys/types.h>
#include<dirent.h>
#include <sys/stat.h>
#include <stdbool.h>
#include<string.h>
#include <time.h>
#include <limits.h>
#include<math.h>

static int LINES;
static int COLS;
static struct dirent **namelist; //for storing the entries in directory
static struct stat *info; //for storing the details of each entry
extern int errorno;

void  populate_data(char *dir_name);
void free_mem(int mem_size, struct stat *info, struct dirent **namelist);
void smart_show(struct stat *info, struct dirent **namelist, int count_entries);
void long_list(struct stat *info, struct dirent **namelist, int count_entries);
int main(int argc, char *argv[])
{
    LINES = atoi(getenv("LINES"));
    COLS = atoi(getenv("COLUMNS"));
    if(system("test -t 0")) //if stdin was redirected
    {
        char filename[PATH_MAX];
        char proclink[PATH_MAX];
        int real_id = getpid();//get the pid 
        sprintf(proclink, "/proc/%d/fd/0", real_id);//get the link of the opened file from fd table..
        int size = readlink(proclink, filename, PATH_MAX);//get the file name using the readlink since they are soft link
        filename[size] = '\0';
        populate_data(filename);
    }
    else if(argc > 1)
    {
        for(int idx = 1; idx < argc; idx++)
        {
            if(argc > 2)
                printf("%s:\n", argv[idx]);
            populate_data(argv[idx]);
            if(argc > 2)
                printf("\n\n");
        }
    }
    else
    {
        populate_data(".");
    }
    return 0;
}
void populate_data(char *dir_name)
{
    struct stat *info;//will hold the information of the file...
    struct dirent **namelist;//will hold the entries of the directory..
    //reading the entries
    int count_entries = 0; // number of entries in the directory..
    count_entries = scandir(dir_name, &namelist, NULL, alphasort); // it will scandir and place the read structure in my namelist
    /*
    int scandir(const char *dirp, struct dirent **namelist,
            int (*filter)(const struct dirent *),
            int (*compar)(const struct dirent **, const struct dirent **));
    here the comparison function is alphasort and Filter is pointed towards NULL
    scandir allocate memory by itself but have to deallocate it
    */
   char path_name[PATH_MAX]; //for storing the path
    if(count_entries < 0)
    {
        perror("scandir");
    }
    else
    {
        info = malloc(count_entries * sizeof(struct stat));
        for(int idx = 0; idx < count_entries; idx++)
        {
            sprintf(path_name, "%s/%s", dir_name, namelist[idx]->d_name);
            int status = lstat(path_name, &info[idx]);
            if(status == -1)
                perror("lstat");
        }
        smart_show(info, namelist, count_entries);
        free_mem(count_entries, info, namelist);
    }
}
void free_mem(int mem_size, struct stat *info, struct dirent **namelist)
{
    for(int idx = 0; idx < mem_size; idx++)
    {
        free(namelist[idx]);
    }
        free(namelist);
        free(info);
}
void smart_show(struct stat *info, struct dirent **namelist, int count_entries)
{
    int max = 0, temp = 0, total = 0;
    for(int idx = 0; idx < count_entries; idx++)
    {
        temp = strlen(namelist[idx]->d_name)  + 1;
        if(temp > max)
        {
            max = temp;
        }
    }
    int cols = (float) COLS / (max + 1);
    int lines = (float) ceil(count_entries / cols);
    printf("%d, %d", cols, (int)ceil(count_entries / cols));
    for(int idx = 0; idx < count_entries; idx++)
    {
        printf(" %s ", namelist[idx]->d_name);
    }
}
void long_list(struct stat *info, struct dirent **namelist, int count_entries)
{
    
}      