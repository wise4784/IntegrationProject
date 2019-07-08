#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(void)
{
	int status;
	pid_t pid;
	
	if((pid=fork())>0){
		wait(&status);
		printf("ls -al Success\n");
	}else if(!pid){
		execlp("ls","ls","-a","-l",0);
	}

	return 0;
}
