#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>

#define	BUFFER_SIZE	1024

int main(int argc, char** argv)
{
	int sock;
	struct sockaddr_in server_addr;
	char buffer[BUFFER_SIZE];
	int str_len;

	if((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("socket failed");
		exit(EXIT_FAILURE);
	}
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = PF_INET;
	server_addr.sin_addr.s_addr = inet_addr(argv[1]);
	server_addr.sin_port = htons(atoi(argv[2]));

	if(connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
		perror("Connect failed");
		close(sock);
		exit(EXIT_FAILURE);
	}

	printf("Input message(Q to quit): ");
	while(1){
		fgets(buffer, BUFFER_SIZE, stdin);
		if(!strcmp(buffer, "Q\n") || !strcmp(buffer, "q\n")) break;
		write(sock, buffer, strlen(buffer));
	}
	/* half-close */
	shutdown(sock, SHUT_WR);

	str_len = read(sock, buffer, BUFFER_SIZE - 1);
	if(str_len > 0) {
		buffer[str_len] = '\0';
		printf("Message from server: %s\n", buffer);
	}

	close(sock);

	return 0;
}
/*
	#include <sys/socket.h>
	int shutdown(int sock, int howto);
*/

