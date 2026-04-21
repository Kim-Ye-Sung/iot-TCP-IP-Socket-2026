#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>

#define	BUFFER_SIZE	1024

int main(int argc, char** argv)
{
	int server_sock, client_sock;
	struct sockaddr_in server_addr, clnt_addr;
	char buffer[BUFFER_SIZE];
	socklen_t clnt_addr_size;
	int str_len;

	if((server_sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("Socket failed");
		exit(EXIT_FAILURE);
	}
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(atoi(argv[1]));

	if(bind(server_sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
		perror("bind failed");
		close(server_sock);
		exit(EXIT_FAILURE);
	}
	if(listen(server_sock, 1) == -1) {
		perror("listen failed");
		close(server_sock);
		exit(EXIT_FAILURE);
	}
	clnt_addr_size = sizeof(clnt_addr);
	puts("Waiting for server .................");
	if((client_sock = accept(server_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size)) == -1) {
		perror("Accept failed");
		close(server_sock);
		exit(EXIT_FAILURE);
	}
	puts("Connected ................");

	while((str_len = read(client_sock, buffer, BUFFER_SIZE)) > 0) {
		write(1, buffer, str_len);
	}

	printf("\n[서버]클라이언트메시지수신완료.응답전송중.....\n");

	char* response = "bye client~~\n";
	write(client_sock, response, strlen(response));
	/*
	for(int i = 0; i < 5; i++) {
		recvlen = recv(client_sock, buffer, sizeof(buffer), 0);
		buffer[recvlen] = '\0';
		printf("recv data: %s\n", buffer);
	}

	//송신종료
	if(shutdown(client_sock, SHUT_WR) == -1) {
		perror("shutdown failed");
		close(client_sock);
		close(server_sock);
		exit(EXIT_FAILURE);
	}

	puts("output stream shutdown ++++++++++++++++++++++");
	if((recvlen = recv(client_sock, buffer, sizeof(buffer), 0)) > 0) {
		buffer[recvlen] = '\0';
		printf("after recv data: %s\n", buffer);
	}
	*/
	close(client_sock);
	close(server_sock);
}
