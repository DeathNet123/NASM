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
        printf("I am Server Waiting for connection...\n");
        int len_addr = sizeof(client_addr);
        client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &len_addr);
        printf("Got connection from the client..\n");

        int rv = 0;
        char buffer[1024];
        char *eof = "";
        while(rv = read(client_socket, buffer, sizeof(buffer)))
        {
            write(client_socket, "ACK\n", 5);
        }
        close(client_socket);
    }
    close(server_socket);
    return 0;
}