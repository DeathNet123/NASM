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
    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);

    //remote server address
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    inet_aton("0.0.0.0", &server_addr.sin_addr);
    server_addr.sin_port = htons(54154);
    memset(&server_addr.sin_zero, '\0', sizeof(server_addr.sin_zero));

    //connecting to remote server
    int status = connect(socket_fd, (struct sockaddr*)&server_addr, sizeof(server_addr));
    if(status == -1)
    {
        perror("connect");
        exit(errno);
    }
    FILE *file = fopen(argv[1], "r");
    
    char *buff = NULL;
    size_t len = 0;
    char name[6];
    int rv = 0;
    while(rv = getline(&buff, &len, file) != -1)
    {   //read from the keyboard..
        write(socket_fd, buff, len);
        read(socket_fd, name, sizeof(name));
        write(1, name, sizeof(name));
    }
    fclose(file);
    free(buff);
    close(socket_fd);
    return 0;
}