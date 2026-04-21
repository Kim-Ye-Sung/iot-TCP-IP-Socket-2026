/* calculate server*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define	BUFFER_SIZE	1024

double calculate(const char*);

int main(int argc, char** argv)
{
	int server_sock, client_sock;
	double result = 0;
	struct sockaddr_in server_addr, client_addr;
	char recvbuf[BUFFER_SIZE];
	char sendbuf[BUFFER_SIZE];
	socklen_t client_addrlen;
	ssize_t bytes_read;

	if((server_sock = socket(PF_INET, SOCK_STREAM, 0)) == 0) {
		perror("Socket failed");
		exit(EXIT_FAILURE);
	}

	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htons(INADDR_ANY);
	server_addr.sin_port = htons(atoi(argv[1]));

	if(bind(server_sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
		perror("Bind failed");
		exit(EXIT_FAILURE);
	}

	if(listen(server_sock, 5) < 0) {
		perror("Listen failed");
		exit(EXIT_FAILURE);
	}

	printf("Server is listening on port %s.....\n", argv[1]);

	if((client_sock = accept(server_sock, (struct sockaddr*)&client_addr, &client_addrlen)) < 0) {
		perror("Accept failed");
		close(server_sock);
		exit(EXIT_FAILURE);
	}

	bytes_read  = recv(client_sock, recvbuf, BUFFER_SIZE, 0);
	recvbuf[bytes_read] = '\0';		//수신데이터끝에null
	printf("Received client: %ld %s\n", bytes_read, recvbuf);

	result = calculate(recvbuf);
//	printf("result: %.2lf\n", result);
	sprintf(recvbuf, "%.2lf", result);
	printf("recvbuf: %s\n", recvbuf);
	send(client_sock, recvbuf, strlen(recvbuf), 0);

	close(client_sock);
	close(server_sock);
}

double calculate(const char* op){
	double result = 0;
	double num1, num2;
	char operator;

	if(sscanf(op, "%lf %c %lf", &num1, &operator, &num2) == 3) {
		switch(operator) {
			case '+' :
				result = num1 + num2; break;
			case '-' :
				result = num1 - num2; break;
			case '*' :
				result = num1 * num2; break;
			case '/' :
				result = num1 / num2; break;
			default:
				result = 0;
				printf("Error"); break;
		}
	}

	return result;
}

/*
	int sscanf(const char* buffer, const char* format, .....);
		문자열(buffeer)로부터포맷에맞는파라미터를저장한다.
	int sprintf(char str, const char format, ....);
*/
