#include<stdio.h>
#include<limits.h>
#include<unistd.h>
int main(void)
{
    char path[PATH_MAX];
    int n = readlink("/bin", path, PATH_MAX);
    path[n] = '\0';
    printf("%s\n", path);
    return 0;
}