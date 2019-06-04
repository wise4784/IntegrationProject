#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(void)
{
	pid_t pid;
	int status;

	if((pid = fork()) > 0)
	{
		wait(&status);

		if(!(status & 0xff))
			printf("(exit) status : %d\n", WEXITSTATUS(status));
		else if(!(status >> 8) & 0xff)
			printf("(signal) status : %d\n", status & 0x7f);
	}
	else if(!pid)
	{
		abort();
	}
	else
	{
		perror("fork() ");
		exit(-1);
	}

	return 0;
}

