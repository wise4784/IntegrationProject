#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <signal.h>


int term_status(int status)
{
    wait(&status);
	if(WIFEXITED(status))  //매크로로 작성된것은 대문자로 작성한다.
		printf("(exit)status: 0x%x\n", WEXITSTATUS(status));
	else if(WTERMSIG(status))
		printf("(signal)status: 0x%x\n", status & 0x7f);
}


void my_sig(int signo)
{
	int status;
	printf("signo = %d\n", signo);  //시그널이 중단된 이유에 따라 다른 코드가 출력됨.
	term_status(status);
}

void my_int(int signo)
{
    printf("signo = %d\n", signo);
    exit(-1);
}


int main(void)
{
	int i;
	pid_t pid;

	signal(SIGCHLD, my_sig); // SIGCHLD가 날라오면 my_sig를 활성화함.  // SIGCHLD는 CHLD가 죽을때 발생됨.
	signal(SIGINT, my_int);
	if((pid = fork()) > 0)
	{
		for(i = 0; i < 10000; i++)
		{
			usleep(50000);  // us로 카운트를 50000까지함.(5초세고 my_sig날림)
			printf("%d\n", i + 1);
		}
	}
	else if(!pid)
	{
		sleep(5);
		abort(); //aort에의해 종료시키게된다 ->다른종료코드뜸.
	}	

	else
	{
		perror("fork() ");
		exit(-1);
	}

	return 0;

}










