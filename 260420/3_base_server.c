#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>

int main(int argc, char* argv[])
{
	int server_fd, client_fd;
	struct sockaddr_in server_addr, client_addr;   //IPv4 관련구조체선언
	socklen_t client_addr_len;
	const char* message = "Hello world!!\n";

	/* IPv4 TCP 서버소켓생성 */
	if((server_fd = socket(PF_INET, SOCK_STREAM, 0)) == -1) {
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
	client_addr_len = sizeof(client_addr);
	if((client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_addr_len)) == -1){
		perror("Accept failed");
		close(server_fd);
		exit(EXIT_FAILURE);
	}

	if(write(client_fd, message, strlen(message)) == -1){
		perror("Write failed");
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

   # 전송함수
   파일디스크립터write()
   ssize_t write(int fd, const void* buf, size_t nbytes);
		nbytes: 전송할바이트수
   소켓전용send()
   ssize_t send(int sockfd, const void* buf, size_t nbytes, int flags);
		nbytes: 전송할바이트수
 */
