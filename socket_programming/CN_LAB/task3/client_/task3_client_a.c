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
#include<linux/limits.h>

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
    
    char buffer[1024];
    char filename[PATH_MAX]; //see the comment in the server file... -__-
    int rv = 0;
    int count = read(0, filename, PATH_MAX);//read the filename..
    filename[count - 1] = '\0';
    write(socket_fd, filename, count);
    int fd = open(filename, O_CREAT | O_RDWR,  S_IRUSR | S_IWUSR);
    while(rv = read(socket_fd, buffer, sizeof(buffer)))
    {
        write(fd, buffer, rv);
    }
    close(socket_fd);
    return 0;
}