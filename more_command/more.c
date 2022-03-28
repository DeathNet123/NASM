#include<stdio.h>
#include <stdlib.h>
#include <time.h>
#include<unistd.h>

static char * LINES = getenv("LINES");
static char * COLS = getenv("COLUMNS");
int main(int argc, char *argv[])
{
    printf("%s %s\n", LINES, COLS);

    return 0;
}