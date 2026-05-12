#include <stdio.h>
#include <unistd.h>

void main(){
	pid_t pid = fork();

	if(pid == 0){	// wktlr vmfhtptj
		puts("I am a child process");  
	
	}else{		// qnahv vmfhtptj
		printf("child process ID: %d\n",pid);
		sleep(30);
	}
	
	if(pid == 0){
	  puts("End Child Process");
	}else{
	  puts("End Parent Process");
	}
	

}
