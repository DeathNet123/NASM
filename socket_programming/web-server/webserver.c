#include<stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include<errno.h>
#include<signal.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<netdb.h>
#include<sys/socket.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<time.h>
#include<wait.h>
#include<unistd.h>
#define MAXREQ 2048
#define MAXRES 2048

void reaper(int signum)
{
    waitpid(-1, NULL, 0);
}

void handle_body(char *required, int client_socket)
{
    struct stat st;
    lstat(required, &st);
    if(((st.st_mode & 0000100) != 0000100) && ((st.st_mode & 0170000) == 0100000))
    {
        int fd = open(required, O_RDONLY);
        char body[MAXREQ];
        int acrv = 0;
        while (acrv = read(fd, body, MAXREQ ))
        {
            write(client_socket, body, acrv);
        }
        close(fd);
    }
    else if(((st.st_mode & 0000100) == 0000100) && ((st.st_mode & 0170000) == 0100000))
    {
        int cpid = fork();
        if(cpid == 0)
        { 
            dup2(client_socket, 1);
            execv(required, NULL);
        }
        else{
            wait(NULL);
        }
            
    }
}

void handle_request(char *request, int client_socket, int rv)
{
        time_t now;
        time(&now);
        char response[MAXRES];//for storing the response header..
        //finding the version number of the request..
        char *v = strstr(request, "HTTP/");
        strtok(v, "\r");//the delim is the carriage return in HTTP/1.1 anyways..
        #ifdef DEBUG
        char ver[10];
        int idx = 0;
        while ((v[idx] >= 65 && v[idx] <= 90) || v[idx] == '/' || (v[idx] >= 48 && v[idx] <= 57) || v[idx] == '.')
        {
            ver[idx] = v[idx];
            idx++;
        }
        #endif

        //finding the resource from the request
        char *resource = strstr(request, "GET");
        strtok(resource, " ");
        char *required = strtok(NULL, " ");

        //Setting up the hostname of the server
        size_t length = 25;
        char hostname[length];
        gethostname(hostname, length);
        
        int test = access(required + 1, F_OK); //checking the existence of the file
        if(test < 0)
        {
            int count = sprintf(response, "%s 404 NOT FOUND\nDate:%s\nServer:%s\n", v, strtok(ctime(&now), "\n"), hostname);
            write(client_socket, response, count);
        }
        else
        { 
            int count = snprintf(response, MAXRES, "%s 200 OK\nDate:%s\nServer:%s\nContent-Type:text/html\n", v, strtok(ctime(&now), "\n"), hostname);
            write(client_socket, response, count); //sending the header first..
            handle_body(required + 1, client_socket);
        }
        return;
              
}

void add_logs(int logs_fd, char *buf, int rv, struct sockaddr_in client_addr)
{
    char log_entry[MAXREQ + 300];
    time_t now;
    time(&now);
    int count = sprintf(log_entry, "%s:%s:%d\n%s", strtok(ctime(&now), "\n"), inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port), buf);
    write(logs_fd, log_entry, count);
}

int main(int argc, char **argv)
{
    signal(SIGCHLD, reaper);
    int server_socket = socket(AF_INET, SOCK_STREAM, 0); //server socket
    int client_socket; //this is client socket
    int logs_fd = open("log.txt", O_APPEND|O_RDWR); //the log file

    struct sockaddr_in server_addr;//server address
    struct sockaddr_in client_addr;//client address

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(54154);//converting the endianess 
    inet_aton(argv[1], &server_addr.sin_addr);
    memset(&server_addr.sin_zero, '\0', sizeof(server_addr.sin_zero));
    bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr));
    listen(server_socket, 10);

    while (1)
    {
        printf("Waiting for the Request...\n");
        int c_len = sizeof(client_addr);
        client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &c_len);
        printf("Request has been received..\n");
        int cpid = fork();
        if(cpid == 0)
        {
            close(server_socket);
            int rv = 0;
            char request[MAXREQ];
            rv = recv(client_socket, request, MAXREQ, 0);
            add_logs(logs_fd, request, rv, client_addr); //adding the logs
            handle_request(request, client_socket, rv);//handling the request
            close(client_socket);
            exit(0);
        }
        else
        {
            close(client_socket);
            continue;
        }
            

    }
    close(server_socket);
    close(logs_fd);
    return 0;
}