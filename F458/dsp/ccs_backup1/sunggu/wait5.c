#include <stdio.h>
#include <signal.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>

void my_sig(int signo)
{
	printf("signo : %d\n", signo);

}

void my_sig2(int signo)
{
	printf("my_sig2 signo: %d\n", signo);
}

int main(void)
{
	void (*old_p)(int);
	void (*old_p2)(int);

	old_p = signal(SIGINT,my_sig);
	pause();

	old_p2 = signal(SIGINT, my_sig2);
	pause();

	old_p2 = signal(SIGINT, old_p);
	pause();

	for(;;)
		pause();

	return 0;
}
