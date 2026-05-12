/* Echo client */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>

#define	BUFFER_SIZE	1024

int main(int argc, char** argv)
{
   int sock_fd;
   struct sockaddr_in server_addr;
   char buffer[1024];
	int recv_len;

	if(argc != 3) {
		printf("usage: %s <ip> <port>\n", argv[0]);
		exit(1);
	}

   if((sock_fd = socket(PF_INET, SOCK_STREAM, 0)) == -1){
      perror("Socket failed");
      exit(EXIT_FAILURE);
   }
   memset(&server_addr, 0, sizeof(server_addr));
   server_addr.sin_family = AF_INET;
   server_addr.sin_addr.s_addr = inet_addr(argv[1]);
   server_addr.sin_port = htons(atoi(argv[2]));

	/* 연결요청*/
   if(connect(sock_fd,(struct sockaddr*) &server_addr, sizeof(server_addr)) == -1) {
      close(sock_fd);
      exit(EXIT_FAILURE);
   }
   puts("Server Conneted.....");

   while(1) {
		fputs("Exit(Q to quit): ", stdout);
		fgets(buffer, BUFFER_SIZE - 1, stdin);
		if(!strcmp(buffer, "q\n") || !strcmp(buffer, "Q\n")) break;

		send(sock_fd, buffer, strlen(buffer), 0);
		recv_len = recv(sock_fd, buffer, sizeof(buffer)-1, 0);
		if(recv_len == -1) printf("Receive failed");
		buffer[recv_len] = '\0';
		printf("Message from server: %s", buffer);
	}

   close(sock_fd);

   return 0;
}
/*

*/

