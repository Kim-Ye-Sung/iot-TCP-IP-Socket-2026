/* Echo serveer */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>

#define	BUFFER_SIZE	1024

int main(int argc, char* argv[])
{
	int server_fd, client_fd;
	struct sockaddr_in server_addr, client_addr;   //IPv4 관련구조체선언
	socklen_t client_addr_size;
	char buffer[BUFFER_SIZE];
	ssize_t bytes_read;

	if(argc != 2) {
		printf("usage: %s <port>\n", argv[0]);
		exit(1);
	}

	/* IPv4 TCP 서버소켓생성 */
	server_fd = socket(PF_INET, SOCK_STREAM, 0);
	if(server_fd == -1) {
		perror("Socket failed");
		exit(EXIT_FAILURE);				// exit(1)
	}

	/* 구조체초기화*/
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;					// IPv4 주소체계
	server_addr.sin_addr.s_addr = INADDR_ANY;			// IP
	server_addr.sin_port = htons(atoi(argv[1]));		// Port

	/* 서버소켓바인딩 */
	if(bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1){
		perror("Bind failed");
		close(server_fd);
		exit(EXIT_FAILURE);
	}

	/* 서버소켓바인딩- 연결대기 */
	if(listen(server_fd, 5) == -1) {
		perror("Listen failed");
		close(server_fd);
		exit(EXIT_FAILURE);
	}

	/* 클라이언트소켓생성- 클라이언트연결수락 */
	client_addr_size = sizeof(client_addr);

	if((client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_addr_size)) == -1){
		perror("Accept failed");
		close(server_fd);
		exit(EXIT_FAILURE);
	}

	while((bytes_read = recv(client_fd, buffer, sizeof(buffer)-1, 0)) != 0){
		buffer[bytes_read] = '\0';
		printf("Message from client: %s", buffer);
		send(client_fd, buffer, bytes_read, 0);
	}

	/* 소켓종료 */
	close(client_fd);
	close(server_fd);

	return 0;
}

/*
	struct sockaddr_in 구조체는IPv4 정보를 취급하기위한구조체
	sin_family, sin_addr, sin_port를 멤버로가지며
	주소체계, 32bit IP주소, 16bit Port 값으로 초기화한다.
	sin_addr는 32bit를 저장하는 in_addr 구조체를 사용하여 초기화한다.

	INADDR_ANY 는 현재실행중인서버IP를소켓에부여한다.

	sockaddr 구조체는 주소정보를담을16byte 구조체

   # 전송함수- nbytes는전송할바이트수
   파일디스크립터write()
   ssize_t write(int fd, const void* buf, size_t nbytes);
   소켓전용send()
   ssize_t send(int sockfd, const void* buf, size_t nbyets, int flags);

   # 수신함수-len은최대버퍼크기
   파일디스크립터 read()
   ssize_t read(int fd, void* buf, size_t len);
   소켓전용 recv()
   ssize_t recv(int sockfd, void* buf, size_t len, int flags);

*/
