#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

void term_status(int status)
{
	if(WIFEXITED(status))
	{
		printf("(exit)status: 0x%x\n", WEXITSTATUS(status));
	}
	else if(WTERMSIG(status))
	{
		printf("(signal)status: 0x%x\n", status & 0x7f);
	}
}

void my_sig(int signo)
{
	int status;

	printf("signo = %d\n", signo);

	//wait : blocking 
	//waitpid : non-blocking 
	//wait(&status);
	while(waitpid(-1, &status, WNOHANG) > 0)
		term_status(status);
}

int main(void)
{
	int i;
	pid_t pid;

	signal(SIGCHLD, my_sig);//함수의 이름은 주소
	// 함수 포인터로 받은 것이라 상관 없다.
#if 0
	blocking인데 non-blocking 처럼 표현하기
	blocking 문제 발생함
	여러번 동시에 신호가 오면 몇 개 처리하다 씹혀버림
	non-blocking 해야 하는데,
	리드할 때 blocking 발생한다고...
	wait(&status)에서 status만 기다리다가 나머지는 어중이 떠중이 돼서
	서버가 터져버린다.
#endif
	if((pid = fork()) > 0)
	{
		for(i=0; i<10000; i++)
		{
			usleep(50000);
			printf("%d\n", i + 1);
		}
	}
	else if(!pid)
	{
		sleep(5);
		abort();
	}
	else
	{
		perror("fork() ");
		exit(-1);
	}

	return 0;
}
