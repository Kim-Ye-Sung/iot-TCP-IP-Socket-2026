#include <stdio.h>

int main(int argc, char *argv[]){
  int i = 0;
  
  printf("number: %d\n", argc);
  printf("program file: %s\n", argv[0]);
  
  for(i = 1; i < argc; i++) {
    printf("추가된 인자 : %s\n", argv[i]);
  }
  
return 0;
}
