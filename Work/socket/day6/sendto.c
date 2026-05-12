#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

int main() {
  int sockfd;
  struct sockaddr_in server_addr;
  char *message = "Hello, UDP!";
  
  //소켓 생성
  sockfd = socket(AF_INET, SOCK_DGRAM, 0);
  
  memset(&server_addr, 0, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(12345);
  inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);
  
  // 데이터 전송
  sendto(sockfd, message, strlen(message), 0, (struct sockaddr*)&server_addr, sizeof(server_addr));
  
  printf("메시지 전송 완료 \n");
  
  close(sockfd);
  return 0;
}
