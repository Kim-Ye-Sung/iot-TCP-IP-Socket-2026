/* calculate client*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define	BUFFER_SIZE	1024

int main(int argc, char** argv)
{
	int sock;
	struct sockaddr_in server_addr;
	char recvbuf[BUFFER_SIZE];
	char sendbuf[BUFFER_SIZE];

	if(argc !=3){
		printf("usage: %s <IP> <port>\n", argv[0]);
		exit(1);
	}

	if((sock = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
		perror("Socket failed");
		exit(EXIT_FAILURE);
	}
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr(argv[1]);
	server_addr.sin_port = htons(atoi(argv[2]));

	if(connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
		perror("Connect failed");
		exit(EXIT_FAILURE);
	}else puts("Connetced.....");

	puts("Enter a mathematical expression");
	fputs("('exit' to quit): ", stdout);
	fgets(sendbuf, BUFFER_SIZE, stdin);
	sendbuf[strlen(sendbuf)-1] = '\0';			// 저장된개행문자-> null
	printf("%s %ld\n", sendbuf, strlen(sendbuf));

	send(sock, sendbuf, strlen(sendbuf), 0);

	recv(sock, recvbuf, BUFFER_SIZE, 0);
	printf("Operation result: %s\n", recvbuf);

	close(sock);

	return 0;
}
/*
	int fputs(const char* str, FILE* stream);
		null까지스트림에복사.null은복사되지않는다.

	char* fgets(char* str, int num, FILE* stream);
		개행,스트림끝,읽은문자-1까지문자를읽는다.개행까지읽어들인다.

	size_t strcspn(const char* str1, const char* str2);
		첫번째로매치되는문자의인덱스를리턴한다.
*/

