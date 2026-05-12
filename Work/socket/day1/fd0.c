#include <stdio.h>

void main()
{
	int fd, fd1, fd2;

	fd = fileno(stdin);
	fd1 = fileno(stdout);
	fd2 = fileno(stderr);

	printf("file descriptor for stdin : %d\n", fd);
	printf("file descriptor for stdout : %d\n", fd1);
	printf("file descriptor for stderr : %d\n", fd2);
}
