#include <stdio.h>
#include <string.h>
#include<arpa/inet.h>
#include <unistd.h>

#define MULTICAST_IP "239.0.0.1"  //멀티캐스트 그룹 IP
#define PORT 5000

int main() {
  int sock;
  struct sockaddr_in addr;
  struct ip_mreq mreq;      // 그룹정보를 저장하는 ip_mreq 구조체
  char buffer[1024];
  
  sock = socket(AF_INET, SOCK_DGRAM, 0);
  
  //포트 재사용
  int reuse = 1;
  setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (char*)&reuse, sizeof(reuse));
  
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr= htonl(INADDR_ANY);
  addr.sin_port = htons(PORT);
  
  bind(sock, (struct sockaddr*)&addr, sizeof(addr));
  
  //멀티캐스트 그룹 가입(수신자)-------
  mreq.imr_multiaddr.s_addr = inet_addr(MULTICAST_IP);
  mreq.imr_interface.s_addr = htonl(INADDR_ANY);
  
  setsockopt(sock, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char*)&mreq, sizeof(mreq));
  //----------------------------------------------------------------------------
  while(1) {
    int len = recvfrom(sock, buffer, sizeof(buffer)-1, 0, NULL, 0);
    buffer[len] = '\0';
    
    printf("Receive: %s\n", buffer);
  }
  
  // 그룹 탈퇴
  setsockopt(sock, IPPROTO_IP, IP_DROP_MEMBERSHIP, &mreq, sizeof(mreq));
  
  close(sock);
  return 0;
}

