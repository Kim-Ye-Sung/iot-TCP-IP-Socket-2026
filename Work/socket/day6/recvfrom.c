#include <stdio.h>
#include <string.h>
#include<arpa/inet.h>
#include <unistd.h>

int main() {
  int sockfd;
  char buffer[1024];
  struct sockaddr_in server_addr, client_addr;
  socklen_t addr_len = sizeof(client_addr);
  
  // 소켓 생성
  sockfd = socket(AF_INET, SOCK_DGRAM, 0);
  
  memset(&server_addr, 0, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = INADDR_ANY;
  server_addr.sin_port = htons(12345);
  
  //바인딩
  bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr));
  
  printf("서버 대기중...\n");
  
  //데이터 수신
  int n = recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr*)&client_addr, &addr_len);
  
  buffer[n] = '\0';
  printf("받은 메시지: %s\n", buffer);
  
  close(sockfd);
  return 0;
}

