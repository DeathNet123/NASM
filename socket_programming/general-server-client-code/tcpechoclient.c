#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>
#include<arpa/inet.h>
#include<errno.h>
extern int errno;
int main(void)
{
    int socketfd = socket(AF_INET, SOCK_STREAM, 0); //Creating the socket..
    //Since it is client adding remote address
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(54154); //change the endianness
    inet_aton("192.168.18.24", &addr.sin_addr);//converting address to binary format..
    memset(&addr.sin_zero, '\0', sizeof(addr.sin_zero));//adding zero in the end
    int rv = connect(socketfd, (struct sockaddr*)&addr, sizeof(addr));//requesting for the connection
    if(rv == -1){perror("connect"); exit(errno);}
    while(1)
    {
        char buf[1024];
        int kbdread = read(0, buf, 1024);//taking input from the user
        buf[kbdread] = '\0';
        int kbdwrite = write(socketfd, buf, strlen(buf));//writing to the socket
        int socketread = read(socketfd, buf, strlen(buf));//reading back from the socket..
        buf[socketread] = '\0';
        write(1, buf, strlen(buf));//writing to the screen
    }
    close(socketfd);
    return 0;
}