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
#include<linux/limits.h>
#include<netdb.h>

int main(int argc, char **argv)
{
    //Sockets fd.. for both client and server
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    int client_socket;

    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;

    //filling the server address...
    server_addr.sin_family = AF_INET;
    inet_aton("0.0.0.0", &server_addr.sin_addr);
    server_addr.sin_port = htons(54154);
    memset(&server_addr.sin_zero, '\0', sizeof(server_addr.sin_zero));
    
    //binding and listening for connections
    bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)); //binding the socket with port
    listen(server_socket, 10); //creating queue...

    
    while(1)
    {
        char filename[PATH_MAX]; //this should not be done since it will make your file server vulnerable to attacks but it is just assignment
        printf("I am Server Waiting for connection...\n");
        int len_addr = sizeof(client_addr);
        client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &len_addr);
        printf("Got connection from the client..\n");
        int count = read(client_socket, filename, PATH_MAX); //reading the file name to be opened...
        filename[count - 1] = '\0';
        int fd = open(filename, O_RDONLY);
        if(fd == -1)
        {
            write(client_socket, "Failed to open file\n", 21);
            write(1, "Failed to open file\n", 21);
            close(client_socket);
            continue;
        }
        int rv = 0;
        char buffer[1024];
        while(rv = read(fd, buffer, sizeof(buffer)))
        {
            write(client_socket, buffer, rv);
        }
        close(client_socket);
    }
    close(server_socket);
    return 0;
}