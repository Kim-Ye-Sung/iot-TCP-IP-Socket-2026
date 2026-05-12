#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

void main()
{
	int fd;
	fd=open("a.txt", O_CREAT  | O_RDONLY);

	if(fd<0)
	{
		perror("could not open a.txt");
		exit(1); // buffer, close, end
	}
	else
	{
		printf("open success\n");
		printf("file discriptor : %d\n", fd);
	}

	close(fd);
}

/*
	#include <fcntl.h>
	int open(const char* name, int flags)
	O_RDONLY, O_WRONLY, O_RDWR, O_AAAEND, O_CREAT
*/
