//Have to Run$ source setting.sh in order to export the lines and columns
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include <sys/types.h>
#include<dirent.h>
#include <sys/stat.h>
#include <stdbool.h>
#include <time.h>

static int LINES;
static int COLS;
static struct dirent **namelist; //for storing the entries in directory
static struct stat *info; //for storing the details of each entry
extern int errorno;

void populate_data(const char *dir_name);
void free_mem(int mem_size);
int main(int argc, char *argv[])
{
    LINES = atoi(getenv("LINES"));
    COLS = atoi(getenv("COLUMNS"));
    if(argc > 1)
    {
        for(int idx = 1; idx < argc; idx++)
        {
            populate_data(argv[idx]);
        }
    }
    else
    {
        int count = populate_data(".");
        free_mem(count);
    }
    return 0;
}
int populate_data(const char *dir_name)
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
    if(count_entries < 0)
    {
        perror("scandir");
    }
    else
    {
        info = malloc(count_entries * sizeof(struct stat));
        for(int idx = 0; idx < count_entries; idx++)
        {
            int status = lstat(namelist[idx]->d_name, &info[idx]);
            if(status == -1)
                perror("stat failed");
            printf("mode: %o\n", info->st_mode);
            printf("link count: %ld\n", info->st_nlink);
            printf("user: %d\n", info->st_uid);
            printf("group: %d\n", info->st_gid);
            printf("size: %ld\n", info->st_size);
            printf("modtime: %ld\n", info->st_mtime);
            printf("name: %s\n", namelist[idx]->d_name);
            printf("------------------------------------\n");
        }
    }
    free_mem(count_entries, info, namelist);
    return;
}
void free_mem(int mem_size, struct stat *info, struct dirent **namelist)
{
    for(int idx = 0; idx < mem_size; idx++)
    {
        free(namelist[idx]);
    }
        free(namelist);
        free(info);
    return;
}       