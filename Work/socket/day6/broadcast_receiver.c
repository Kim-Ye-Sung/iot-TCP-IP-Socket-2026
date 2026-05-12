#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

int main(int argc, char *argv[]){
  int sock;
  struct sockaddr_in addr;
  char buffer[1024];
  
  if(argc!=2) {
    printf("Usage: %s <PORT>\n", argv[0]);
    exit(1);
  }
  
  sock = socket(AF_INET, SOCK_DGRAM, 0);
  
  addr.sin_family = AF_INET;
  addr.sin_port = htons(atoi(argv[1]));
  addr.sin_addr.s_addr = INADDR_ANY;  // 모든 인터페이스에서 수신
  
  bind(sock, (struct sockaddr*)&addr, sizeof(addr));
  
  while(1){
    int len = recv(sock, buffer, sizeof(buffer)-1, 0);
    buffer[len] = '\0';
    
    printf("recv: %s\n", buffer);
  }
  
  close(sock);
  return 0;
}
