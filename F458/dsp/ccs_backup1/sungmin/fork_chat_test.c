#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

int main(void){
	int fd, ret;
	char buf[1024];
	pid_t pid;

	fd=open("myfifo",O_RDWR);

	if((pid=fork())>0){
		for(;;){
			ret=read(0,buf,sizeof(buf));
			buf[ret]=0;
			printf("Keyboard: %s\n",buf);
		}
	}
	else if(!pid){
		for(;;){
			ret=read(fd,buf,sizeof(buf));
			buf[ret]=0;
			printf("Pipe: %s\n",buf);
		}
	}
	else{
		perror("fork()");
		exit(-1);
	}
	close(fd);
}
