#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<fcntl.h> 
int main(void)
{
	int idx = 0;
	char a[10] = "987654321\n";
	int fd = open("file.txt", O_WRONLY | O_CREAT | O_EXCL );
	while(idx < 10)
	{
		write(fd, a, 10);
		idx++;	
	}
	return 0;
}

