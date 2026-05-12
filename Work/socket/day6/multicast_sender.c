#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

#define MULTICAST_IP "239.0.0.1"
#define PORT 5000

int main() {
  int sock;
  struct sockaddr_in addr;
  char message[] = "Hello Multicast";
  
  sock = socket(AF_INET,SOCK_DGRAM, 0);
  
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = inet_addr(MULTICAST_IP);
  addr.sin_port = htons(PORT);
  
  while(1) {
    sendto(sock, message, strlen(message), 0, (struct sockaddr*)&addr, sizeof(addr));
    
    printf ("Sent: %s\n", message);
    sleep(1);
  }
  
  close(sock);
  return 0;
}
