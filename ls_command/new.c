#include<stdio.h>
#include<limits.h>
#include<unistd.h>
int main(void)
{
    int x = 10;
    int *ptr_x = (int *) malloc(x * sizeof(int));
    for(int idx = 0; idx < x; idx++) ptr_x[idx] = (int *) malloc(sizeof(int) * x);
    printf("%d\n", **ptr_x_x);
    char path[PATH_MAX];
    int n = readlink("/bin", path, PATH_MAX);
    path[n] = '\0';
    printf("%s\n", path);
    return 0;
}