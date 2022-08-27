#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<errno.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<netdb.h>

extern int errno;
int main(int argc, char **argv)
{
    //socket for communication..
    int socket_fd = socket(AF_INET, SOCK_DGRAM, 0);

    //remote server address
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    inet_aton("0.0.0.0", &server_addr.sin_addr);
    server_addr.sin_port = htons(54154);
    memset(&server_addr.sin_zero, '\0', sizeof(server_addr.sin_zero));

    while(1)
    {
        int len = 0;
        char buf[1024];
        char name[1024];
        memset(buf, '\0', sizeof(buf));
        memset(name, '\0', sizeof(name));
        len = read(0, buf, sizeof(buf)); //read from the keyboard..
        sendto(socket_fd, buf, len, 0, (struct sockaddr*)&server_addr, sizeof(server_addr)); //write to the socket_fd..
        int size = sizeof(server_addr);
        len = recvfrom(socket_fd, name, sizeof(name), 0, (struct sockaddr*)&server_addr, &size);//read from the socket_fd..
        write(1, name, len);//write on the screen...
    }
    close(socket_fd);
    return 0;
}