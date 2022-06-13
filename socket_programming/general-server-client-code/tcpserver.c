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

int main(void)
{
    int s_socket;
    int c_socket;
    
    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;
    
    s_socket = socket(AF_INET, SOCK_STREAM, 0); //socket created for the server to listen...
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(54154);
    inet_aton("172.22.74.129", &server_addr.sin_addr);
    memset(&server_addr.sin_zero, '\0', sizeof(server_addr.sin_zero));
    bind(s_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)); //binding the socket with port
    
    listen(s_socket, 10); //creating queue and 
    while (1)
    {
        printf("Waiting for the connection...\n");
        int c_len = sizeof(client_addr);
        c_socket = accept(s_socket, (struct sockaddr*)&client_addr, &c_len);
        printf("Connect Established with client...\n");
        int rv = 0;
        char buf[1024];
        while(rv = read(c_socket, buf, 1024))
            write(c_socket, buf, rv);
        printf("Client connection terminated...\n");
        close(c_socket);
    }
    close(s_socket);
    exit(0);
    return 0;
}
