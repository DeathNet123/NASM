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

void search(char **data, char *key, int fd)
{
    for(int idx = 0; data[idx] != NULL; idx++)
    {
        if(strstr(data[idx], key) != NULL)
        {
            int len = strlen(strstr(data[idx], ",") + 1);
            write(fd, strstr(data[idx], ",") + 1, len);
            return;
        }
    }
    write(fd, "RECORD NOT FOUND\n", 18);
}
int main(int argc, char **argv)
{
    char *data[6];
    data[0] = "BCSF19M556,Arslan Ahmed Qureshi\n";
    data[1] = "BCSF19M534,Rana Faran Abdullah\n";
    data[2] = "BCSF19M513,Ali Raza mumtaz\n";
    data[3] = "BCSF19M526,Sarmad Khalique\n";
    data[4] = "BCSF19M550,Hadeed Butt\n";
    data[5] = NULL;
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

        while(rv = read(client_socket, buffer, sizeof(buffer)))
        {
            buffer[rv - 1] = '\0';
            if(rv < 10)
            {
                write(client_socket, "WRONG FORMAT\n", 14);
                continue;
            }
                
            search(data, buffer, client_socket);
        }
        close(client_socket);
    }
    close(server_socket);
    return 0;
}