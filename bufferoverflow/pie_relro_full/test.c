#include<stdio.h>
#include <string.h>
#include<unistd.h>

void getmessage(void)
{
    char msg[200];
    printf("Enter the message: ");
    scanf(" %[^\n]s", msg);
    for(int idx = 0; idx < strlen(msg); idx++)
    printf("%x ", msg[idx]);
}
int main(void)
{
    printf("%lx");
    getmessage();
    return 0;
}