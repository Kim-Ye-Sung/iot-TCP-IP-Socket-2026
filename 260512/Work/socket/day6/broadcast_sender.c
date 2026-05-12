#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char *argv[]){
  int sock;
  struct sockaddr_in addr;
  int broadcastEnable = 1;
  
  if(argc != 3){
    printf("Usage : %s <Broadcast IP> <PORT>\n", argv[0]);
    exit(1);
  }
  
  sock =socket(AF_INET, SOCK_DGRAM, 0);
  
  //브로드캐스트 허용 옵션(중요)
  setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &broadcastEnable, sizeof(broadcastEnable));
  
  addr.sin_family = AF_INET;
  addr.sin_port = htons(atoi(argv[2]));
  addr.sin_addr.s_addr = inet_addr("255.255.255.255");  //전체 네트워크 대상
  
  char *msg = "Hello Broadcast";
  
  sendto(sock, msg, strlen(msg), 0 ,(struct sockaddr*)&addr, sizeof(addr));
  
  close(sock);
  return 0;
}
