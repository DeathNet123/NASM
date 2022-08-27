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

void search(char **data, char *key, struct sockaddr *address, int server_socket, int size)
{
    printf("%s", key);
    for(int idx = 0; data[idx] != NULL; idx++)
    {
        if(strstr(data[idx], key) != NULL)
        {
            printf("I was here!\n");
            int len = strlen(strstr(data[idx], ",") + 1);
            sendto(server_socket, strstr(data[idx], ",") + 1, len, 0, address, size);
            return;
        }
    }
    sendto(server_socket, "RECORD NOT FOUND\n", 18, 0, address, size);
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
    int server_socket = socket(AF_INET, SOCK_DGRAM, 0);
    struct sockaddr_in server_addr;

    //filling the server address...
    server_addr.sin_family = AF_INET;
    inet_aton("0.0.0.0", &server_addr.sin_addr);
    server_addr.sin_port = htons(54154);
    memset(&server_addr.sin_zero, '\0', sizeof(server_addr.sin_zero));
    
    //binding and listening for connections
    bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)); //binding the socket with port
    while(1)
    {
        printf("I am Server Waiting for The roll no...\n");
        char buffer[1024];
        int addr_len = sizeof server_addr;
        memset(buffer, '\0', sizeof(buffer));
        int rv = recvfrom(server_socket, buffer, sizeof(buffer), 0, (struct sockaddr*)&server_addr, &addr_len); //read from the server_socket
        buffer[rv - 1] = '\0';
        if(rv < 10)
        {
            sendto(server_socket, "WRONG FORMAT\n", 14, 0, (struct sockaddr*)&server_addr, addr_len);
            continue;
        }
        search(data, buffer, (struct sockaddr*)&server_addr, server_socket, addr_len);
    }
    close(server_socket);
    return 0;
}