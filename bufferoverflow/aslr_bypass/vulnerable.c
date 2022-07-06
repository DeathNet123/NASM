#include<stdio.h>
#include <string.h>
#include<unistd.h>

void getmessage(void)
{
    char msg[200];
    printf("Enter the message: ");
    scanf(" %[^\n]s", msg);
    printf("Message received.\n");
}
int main(void)
{
    getmessage();
    return 0;
}